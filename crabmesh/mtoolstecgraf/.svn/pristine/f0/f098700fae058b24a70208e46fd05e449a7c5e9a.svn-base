/*
** ---------------------------------------------------------------
**
** JmeshAdvnFrt3D.c - Advancing Front module.
**
** ---------------------------------------------------------------
**
** Description:
**  Routines to implement mesh3D generation based in a boundary
**  contraction (advancing front) technique using an octree as
**  the density function for the creation of new interior nodes.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**   It was based on Wash's and Tilio's initial implementation.
**  Modified:	Mar-10-1999	Joaquim Bento Cavalcante Neto
**   Code updated to include two versions regarding the way the
**   front advances: one based on using as base face always the
**   smallest face in the front (SMALL) and other based on using
**   as base face the faces from the boundary towards the interior
**   of the domain (ADVAN).
**  Modified:	Oct-18-1999	Bruce Carter
**   Included both versions (ADVAN and SMALL) in only one, defined
**   by the variable jmesh_version.
**  Modified:	Dec-09-1999	Joaquim Bento Cavalcante Neto
**   The jmesh_version variable, instead of being an external one
**   as defined before, now is a global static one which assumes
**   the value of the new variable version passed through the
**   function Msh3DBdryContraction.
**  Modified:	Oct-25-01	Joaquim Bento Cavalcante Neto
**   Modified function Msh3DTreeNode that now generates a new node
**   based exactly on the size of the cell where the center of the
**   face is. This will allow to have a better control over the
**   degree of the mesh refinement by changing the factor used to
**   build the octree. This factor is an input data for jmesh and
**   can be changed.
**  Modified:   Oct-09-02	Joaquim Bento Cavalcante Neto
**   Changed the code to give more feedback on how the generation is
**   going, in order to help the user during the mesh generation.
**  Modified:	Feb-17-03	Joaquim Bento Cavalcante Neto
**   Included AD_TREE and PT_TREE to speed up face and node search.
**   The procedures can be turned on/off by #defines on the code.
**   Also included #defines started with word OLD to implement new
**   procedures but to save the old ones. The old version can be
**   retrieved by turning all the OLD #defines on (making them 1).
**  Modified:	Feb-19-2003     Joaquim Bento Cavalcante Neto
**   Modified struct Msh3DBdryNodeRec to include mates, which will
**   define the mate nodes, if any, of a given node, in case of cracks.
**   The first position of the mate vector (mates[0]) is the number
**   of the mates and the remaining positions are the ids of the mates.
**   Changed the places where the query memory is released, by
**   calling functions QueryCleanRangeTree and QueryCleanPointTree
**   created for this purpose. The motive is that now the memory
**   is allocated and freed by the trees and not in this module.
**  Modified:	Mar-26-2003	Joaquim Bento Cavalcante Neto
**   Included a new OLD define, called #OLD_NORM, to change tol.
**   Changed all geo functions that now starts with Geo instead.
**   Changed the timing for back-tracking and deleting phases,
**   that now print timing after each phase and not at the end.
**   Changed name of function Msh3DElemFace to Msh3DNoElemFace.
**   Changed name of function Msh3DCheckFace to Msh3DBoundaryFace.
**   Included diagnosis defines (all of them starting with #DIA).
**   Changed name of variable ntest to push_version. Also changed
**   function Msh3DPushSmall to include the option #OLD_PUSH on it.
**   Included procedures for adjacency, smallest edge on the model
**   and mate vector construction in one single loop to speed up.
**   Changed function Msh3DPushBdryFace to include layer parameter.
**   Modified function Msh3DCrtnFDelF to delete repeated faces that
**   might be included in a polyhedron when it reaches another one.
**   Created function Msh3DEqualFaces to check if faces are equal.
**   Created function Mesh3DDiaDelf to diagnosis reconstruction of
**   polyhedron when two identical faces are included back on it.
**   Changed Msh3DFindPolyBuild to deal also with the pathological
**   case of two polyhedra sharing one edge. In this case we want
**   to treat each polyhedron, in the back-tracking, separately.
**   Created Msh3DRecoPoly routines for polyhedron reconstuction.
**   Created defines VER_BOTH and VER_PRNT to use the two versions.
**   Also created functions Msh3DNodeFreeAll, Msh3DRangeFreeAll,
**   and Msh3DPointFreeAll for the same purpose, as well as
**   introduced a procedure to return a error status after the
**   algorithm has tried to generate elems after a certain number
**   of times in the back-tracking phase. Changed function
**   Msh3DNoElemFace to include new #define called OLD_ELEM.
**   Changed Msh3DBoundaryFace to include new #define OLD_BDRY.
**   Created a function Msh3DCreationFace to check if a boundary
**   face will be reinserted in the polyhedron for back-tracking.
**   Created function Msh3DDelRepeatedFace to delete repeated faces
**   in the polyhedron when the current polyhedron reaches another
**   one in the vicinity and then they should be joined as only one.
**   Created a new define OLD_NEAR to avoid near faces insertion.
**   Created a new function Msh3DCheckInbox to check if new nodes
**   are inside the octree box, otherwise we're meshing outwards.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ----------------------------------------------------------------
*/

#include "PJmesh/JmeshMainDef3D.h"
#include "PJmesh/JmeshRangeTree.h"
#include "PJmesh/JmeshPointTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//commented by Markos
//#define SMALL_VERSION	0
//#define ADVAN_VERSION	1
//end commented by Markos

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC  1.0e+06
#endif

/* --------------------------------------------------------------
** Global variables:
*/

extern int                jmesh_version ;
extern Msh3DBdryNodeList  node_list     ;
extern int                node_alloc    ;
extern int                node_quantum  ;
//extern Msh3DBdryFace      bdry_stack    ;	//Markos - inicio da lista de faces da fronteira
//extern Msh3DPolyFace      poly_stack    ;
extern int                nintnode      ;
extern int                nbdrynode     ;
extern int                GdbWait       ;
extern int                DebugFail     ;
extern int                int_offset    ;
extern double             smallest_area ;

/* Markos
	Esquema da lista de faces da fronteira:

	inicio         fim da borda  topologia     back-tracking       fim
	bdry_stack ... bdry_last ... bdry_use1 ... bdry_use2 ... bdry_tail
	                             bdry_try
*/

/* --------------------------------------------------------------
** Definitons for use of algorithms:
*/

#define DSP_TIME	1	/* Print the execution time  */
#define DSP_DELE	0	/* Display for shape  phase  */
#define DSP_PRNT	0	/* Print messages for jmesh  */
#define DSP_DRAW	0	/* Redraw front for n faces  */
#define DSP_NBAR	0	/* Draw face's optimal node  */
#define DSP_NADJ	0	/* Draw face's adjacent node */

#define AD_TREE		1	/* Use   AD_TREE for face search */
#define AD_DEBU		0	/* Debug AD_TREE for face search */

#define PT_TREE		1	/* Use   PT_TREE for node search */
#define PT_DEBU		0	/* Debug PT_TREE for node search */

//removed by markos
//#define VER_BOTH        1       /* Try Jmesh with both versions */
//#define VER_PRNT        0       /* Print Jmesh warning messages */
//end removed by markos

#define OLD_INTE	1	/* Intersection using Geo package  */
#define OLD_TOLE	1	/* Intersection using fixed tol    */
#define OLD_NULL	1	/* Check generation of null area   */
#define OLD_ELEM	1	/* Search for all elements on mesh */
#define OLD_SMOO	1	/* Element adjacency for all nodes */
#define OLD_DELE	1	/* Deleting for shape improvement  */
#define OLD_BDRY        1       /* Use search for boundary faces   */
#define OLD_FIND	0	/* Use different find polyhedron   */
#define OLD_PUSH	0	/* Push small face using fixed tol */
#define OLD_BTRY	0	/* Find bdry_try using search      */
#define OLD_TRAN	0	/* No transition check for elems   */
#define OLD_NORM	0	/* Use smaller tolerance for norm  */
#define OLD_POLY	0	/* Use old find polyhedron method  */
#define OLD_RECO	0	/* No  reconstruction in CrtnFDelF */
#define OLD_VISI        0       /* No saving of face unvisibility  */
#define OLD_FIXE	0	/* No fixing for repeated faces    */
#define OLD_NEAR	0	/* Check creation near boundary    */
#define OLD_TEST	0	/* Check recreation of old faces   */
#define OLD_INSI	0	/* Check creation outside octree   */
#define OLD_SMAL	0	/* No next checking in PushSmall   */

#define DIA_BACK	0	/* Diagnosis for elem deletion   */
#define DIA_POLY	0	/* Diagnosis for node polyhedron */
#define DIA_NEAR	0	/* Diagnosis for near boundary   */
#define DIA_FIND	0	/* Diagnosis for find polyhedron */
#define DIA_BTRY	0	/* Diagnosis for bdry_try search */
#define DIA_RECO	0	/* Diagnosis for Msh3DCrtnFDelF  */
#define DIA_FIXE        0       /* Diagnosis for fixing faces    */
#define DIA_PUSH	0	/* Diagnosis for Msh3DBdryPush   */
#define DIA_INSI	0	/* Diagnosis for outside octree  */

/* --------------------------------------------------------------
** Private definitons and data types:
*/

#define Msh3D_NO_VALID_NODES		2
#define Msh3D_INSERT_NO_VALID_NODES     3
#define Msh3D_NO_ELEMENT_EXTRACTION     4
#define Msh3D_SMOOTH_STEP               4
#define Msh3D_DELETE_STEP               4
#define Msh3D_DELETE_ELEM               20
#define Msh3D_DELETE_MEMO               200
#define MSH3D_POLY_FACE_CORRE           900
#define MSH3D_POLY_FACE_QUANTUM         10000
#define MSH3D_BDRY_FACE_QUANTUM		10000
#define MSH3D_TEST_FACE_QUANTUM		10000
#define MSH3D_ADJ_ELEM_QUANTUM 		10000
#define MSH3D_ADJ_FACE_QUANTUM 		10000
#define MSH3D_ADJ_INI_FACE_QUANTUM      10000
#define MSH3D_ELEM_QUANTUM 		10000
#define Msh3D_BDRY_STACK_FACTOR		3
#define Msh3D_BDRY_FACE_DRAW            500
#define TOL_FACTOR	        	0.0001 /* changed by tol_factor */
#define TOL_INTERS			1.0e-3 /* changed by tol_inters */
#define SPH_FACTOR                      05.0
#define SHA_FACTOR                      8.5
#define MUL_FACTOR                      30.0
#define REL_FACTOR                      1.0
#define NEW_NODES			2
#define POL_NODES			8
#define PI 				3.141592653589793324
#define ABOUT_ZERO(val,tol)             ((val<tol)&&(val>(-tol)))
#define OUT_FACE(val,tol)               ((val>(1.0+tol))||(val<(0.0-tol)))
#define MAX(a,b)                        (((a)>(b))?(a):(b))
#define MIN(a,b)                        (((a)<(b))?(a):(b))
#define SIGN(dot)                       (((dot)>0.0)?(1):(-1))
#define TRUE				1
#define FALSE				0
#define MSH3D_ADTREE_QUANTUM 		5000
#define FACE_TOLERANCE			1e-5

/* --------------------------------------------------------------
** Private variables:
*/

static char               Msh3D_err_msg[80] ;
static int                Msh3D_errno ;
static int                push_version = 0 ;
static int                num_elem_alloced = 0 ;
static int                num_elem_deleted = 0 ;
static int                num_elem_created = 0 ;
static int                num_elem_studied = 0 ;
static Msh3DBdryFace      test_free = 0 ;
static Msh3DBdryFace      test_block_ptr = 0 ;
static Msh3DBdryFace      test_tail = 0 ;
static Msh3DBdryFace      test_cursor = 0 ;
//static Msh3DBdryFace      bdry_free = 0 ;			//Markos - inicio da lista de faces livres (já alocadas mas não mais utilizadas)
//extern Msh3DBdryFace      bdry_free ;			//Markos - inicio da lista de faces livres (já alocadas mas não mais utilizadas)
//static Msh3DBdryFace      bdry_block_ptr = 0 ;	//Markos -
//extern Msh3DBdryFace      bdry_block_ptr ;		//Markos -
//static Msh3DBdryFace      bdry_tail = 0 ;			//Markos - fim da lista de faces da fronteira
//extern Msh3DBdryFace      bdry_tail ;			//Markos - fim da lista de faces da fronteira
//static Msh3DBdryFace      bdry_last = 0 ;			//Markos - ultima face do contorno original (da fronteira original dada como entrada)
//extern Msh3DBdryFace      bdry_last ;			//Markos - ultima face do contorno original (da fronteira original dada como entrada)
//static Msh3DBdryFace      bdry_cursor = 0 ;		//Markos - ponteiro utilizado para percorrer as faces da fronteira
//extern Msh3DBdryFace      bdry_cursor ;			//Markos - ponteiro utilizado para percorrer as faces da fronteira
//static Msh3DBdryFace      bdry_try = 0 ;			//Markos - faces entre bdry_stack e bdry_try são da fase geometrica, faces entre bdry_try e bdry_last são da fase topológica ou da fase de back-tracking
//extern Msh3DBdryFace      bdry_try ;			//Markos - faces entre bdry_stack e bdry_try são da fase geometrica, faces entre bdry_try e bdry_last são da fase topológica ou da fase de back-tracking
//static Msh3DBdryFace      bdry_use1 = 0 ;			//Markos - primeira face a ser futuramente usada pela fase topológica
//extern Msh3DBdryFace      bdry_use1 ;			//Markos - primeira face a ser futuramente usada pela fase topológica
//static Msh3DBdryFace      bdry_use2 = 0 ;			//Markos - primeira face a ser futuramente usada pela fase de back-tracking
//extern Msh3DBdryFace      bdry_use2 ;			//Markos - primeira face a ser futuramente usada pela fase de back-tracking
static Msh3DBdryFace      bdry_useNeg = 0 ;			//Markos - primeira face que nao pode avancar, pelo pjmesh
//static Msh3DPolyFace      poly_free = 0 ;
//static Msh3DPolyFace      poly_block_ptr = 0 ;
//static Msh3DPolyFace      poly_tail = 0 ;
//static Msh3DPolyFace      poly_cursor = 0 ;
static Msh3DAdjElem       adj_elem_dele = 0 ;
static Msh3DAdjElem       adj_elem_free = 0 ;
static Msh3DAdjElem       adj_elem_block_ptr = 0 ;
static Msh3DAdjFace       adj_free = 0 ;
static Msh3DAdjFace       adj_block_ptr = 0 ;
static Msh3DAdjIniFace    adj_ini_free = 0 ;
static Msh3DAdjIniFace    adj_ini_block_ptr = 0 ;
static long               init_time, tot_time;
static long               init_tree_time, tot_tree_time;
static int                adtreeindx = 0 ;
static int                adtreesize = 0 ;
static Msh3DBdryFace     *adtreehash = 0 ;
static int                startid = 0 ;

/* --------------------------------------------------------------
** Private functions prototypes:
*/

static int             Msh3DSmooth() ;
static Msh3DPolyFace   Msh3DPushPolyFace() ;
static Msh3DBdryFace   Msh3DPushBdryFace() ;
static int             Msh3DTmp1() ;
static int             Msh3DTmp2() ;
static int             Msh3DTmp3() ;
static int             Msh3DTmp4() ;
static int             Msh3DTmp5() ;
static int             Msh3DTmp6() ;
static int             Msh3DTmp7() ;
static int             Msh3DTmp8() ;
static int             Msh3DTmp9() ;
static int             Msh3DTmp10() ;
static int             Msh3DTmp11() ;
static double          Msh3DVolum() ;
static double          Msh3DSrms() ;
static int             Msh3DCheckPoly() ;
static int             Msh3DCheckValid() ;
static int             Msh3DCheckVolum() ;
static int             Msh3DCheckOptim() ;
static int             Msh3DCheckShape() ;
static int             Msh3DCheckVisib() ;
static int             Msh3DCheckPoint() ;
static int             Msh3DCheckConsi() ;
static int             Msh3DCheckTrans() ;
static int             Msh3DCheckInbox() ;
static int             Msh3DDetNode() ;
static void            Msh3DAddFaces() ;
static void            Msh3DAddPolyFaces() ;
static void            Msh3DHeapInit() ;
static void            Msh3DHeapDelete() ;
static void            Msh3DHeapInsert() ;
static int             Msh3DHeapExtract() ;
static double          Msh3DHeapSolidAngle() ;
static double          Msh3DHeapSolidAngleTria() ;
static double          Msh3DHeapSolidEdge() ;
static Msh3DBdryFace   Msh3DTestAlloc() ;
static Msh3DBdryFace   Msh3DTestPush() ;
static void            Msh3DTestFree() ;
static void            Msh3DTestFreeAll() ;
static Msh3DPolyFace   Msh3DPolyFaceAlloc() ;
static void            Msh3DPolyFree() ;
static void            Msh3DPolyFaceFreeAll() ;
static void            Msh3DPolyPush() ;
static Msh3DPolyFace   Msh3DPolyPop() ;
static void            Msh3DPolyDelete() ;
static Msh3DBdryFace   Msh3DFaceAlloc() ;
static void            Msh3DBdryFree() ;
//static void            Msh3DFaceFreeAll() ;
static void            Msh3DBdryPush() ;
static void            Msh3DBdryPushFront() ;
static void            Msh3DBdryPushCorre() ;
static void            Msh3DBdryPushSmall() ;
static void            Msh3DBdryPushBack() ;
static Msh3DBdryFace   Msh3DBdryPop() ;
static Msh3DBdryFace   Msh3DBdryPopCorre() ;
static void            Msh3DBdryDelete() ;
static Msh3DAdjFace    Msh3DAdjFaceAlloc() ;
static void            Msh3DAdjFree() ;
static void            Msh3DAdjFreeAll() ;
static Msh3DAdjIniFace Msh3DAdjIniFaceAlloc() ;
static void            Msh3DAdjIniFree() ;
static void            Msh3DAdjIniFreeAll() ;
static Msh3DAdjElem    Msh3DAdjElemAlloc() ;
static void            Msh3DAdjElemFree() ;
static void            Msh3DAdjElemFreeAll() ;
static void            Msh3DAdjNodeFreeAll() ;
static void            Msh3DNodeFreeAll() ;
static void            Msh3DRangeFreeAll() ;
static void            Msh3DPointFreeAll() ;
static void            Msh3DAddElem() ;
static void            Msh3DBdryDump() ;
static Msh3DBdryFace   Msh3DFindFace() ;
static int             Msh3DBoundaryFace() ;
static int             Msh3DEqualFaces () ;
static int             Msh3DNoElemFace() ;
static int             Msh3DVerifFace() ;
static int             Msh3DCreationFace() ;
static int             Msh3DDelRepeatedFace() ;
static int             Msh3DElemPoly() ;
static int             Msh3DStarPoly() ;
static int             Msh3DVisiPoly() ;
static void            Msh3DFindPoly() ;
static void            Msh3DRecoPoly() ;
static void            Msh3DFindNode() ;
static void            Msh3DTreeNode() ;
static void            Msh3DPushNode() ;
static int             Msh3DInsertNewNodesKer() ;
static int             Msh3DInsertNewNodesAdj() ;
static int             Msh3DInsertNewNodesBar() ;
static int             Msh3DInsertNewNodesCen() ;
static int 	       Msh3DInsertNewNodesPol() ;
static int 	       Msh3DInsertNewNodesBdr() ;
static void	       Msh3DInsertNewNodesOct() ;
static Msh3DBdryFace   Msh3DCrt1FDelV() ;
static Msh3DBdryFace   Msh3DCrt2FDelE() ;
static Msh3DBdryFace   Msh3DCrt3FDelF() ;
static void            Msh3DCrtnFDelV() ;
static void            Msh3DCrtnFDelF() ;
static int	       Msh3DDecomPoly() ;
static int	       Msh3DCorrePoly() ;
static int	       Msh3DShapePoly() ;
static void            Mesh3DDiaDelf () ;
static int             GeoTriIntersect() ;
static int             GeoTriSegIntersect() ;
static int             GeoTriSegIntersectCheck() ;
static int             GeoSegSegIntersect() ;
static double          GeoNorm() ;
static double         *GeoCrossProd() ;
static double         *GeoCross() ;
static double         *GeoCrossNorm() ;
static double          GeoDot() ;
static int             FaceIntersectCoPlanar() ;
static int             FaceIntersect() ;
static int             CheckHasElement () ;
static void            IntersectFaceWithPlane () ;
static int             CheckInPlaneIntsct () ;
static void            ThreePlaneIntersect () ;
static int             LineFaceIntersect () ;
static int             CheckCross () ;
static double         *CrossProd () ;
static double         *TripleCrossProd () ;

/* --------------------------------------------------------------
** Public functions prototypes:
*/

/*void           Msh3DPolyReset() ;
Msh3DPolyFace  Msh3DPolyNext() ;
void           Msh3DBdryReset() ;
Msh3DBdryFace  Msh3DBdryNext() ;*/

/* ----------------------------------------------------------
** Msh3DError - return error information.
*/

/*void Msh3DError( number, text )
    int    *number ;
    char   **text ;
{
    *number = Msh3D_errno ;
    *text = Msh3D_err_msg ;
}*/

//added by Markos
//#define ENABLE_BACK_TRACKING			(0)
//#define ENABLE_IMPROVING				(1)
//#define ENABLE_SMOOTHING				((ENABLE_IMPROVING) && (1))
//#define ENABLE_IMPROVING_BACK_TRACKING	((ENABLE_IMPROVING) && (ENABLE_BACK_TRACKING) && (1))

//static int PMshCountPartition = 0;
//static int PMshStudiedPartition = 22;

struct PMsh3DInvertedFace
{
	Msh3DBdryFace face;
	struct PMsh3DInvertedFace *next;
};

static struct PMsh3DInvertedFace *pmsh3d_inverted = 0;

static int PMsh3DOutBoxPt(double pt[3], double box_min[3], double box_max[3])
{
	int i = 0;

	for (i = 0; i < 3; i++)
	{
		if ((pt[i] <= box_min[i] - tol_factor) || (pt[i] >= box_max[i] + tol_factor))
		//if ((pt[i] <= box_min[i]) || (pt[i] >= box_max[i]))
		//if ((pt[i] < box_min[i] - tol_factor) || (pt[i] > box_max[i] + tol_factor))
		{
			return 1;
		}
	}

	return 0;
}

static int PMsh3DOutBox(double imin[3], double imax[3], double box_min[3], double box_max[3])
{
	double min[3], max[3];
	int i;

	for (i = 0; i < 3; i++)
	{
		min[i] = MAX(imin[i], gmin[i] + tol_factor);
		max[i] = MIN(imax[i], gmax[i] - tol_factor);
	}

	return (PMsh3DOutBoxPt(min, box_min, box_max) || PMsh3DOutBoxPt(max, box_min, box_max));
}

static int PMsh3DCheckExist(Msh3DBdryFace face, Msh3DBdryNodeList node_list, int node_indx, int pjmesh_out_pt)
{
	int v1i, v2i, v1, v2, i;
    int found;
    Msh3DAdjFace cur;

	if ((pjmesh_out_pt == 0) || (face->use == 0))
	{
		return 1;
	}

	for (v1i=1, v2i=0; v2i < 3; v1i = ((v1i + 1)%3), v2i++)
	{
		v1 = face->verts[v1i];
		v2 = face->verts[v2i];

        for (cur = node_list[v1].faces; cur; cur = cur->next)
		{
            for (i = found = 0; (i < 3) && !found; i++)
            {
                if ((cur->face->verts[i] == v1) &&
                    (cur->face->verts[(i+1)%3] == v2) &&
                    (cur->face->verts[(i+2)%3] == node_indx))
				{
					found = 1;
				}
            }

            if (found)
            {
            	break;
            }
        }

        if (!found)
		{
			/*face->use = -1;
			//face->use++;
			if (face->use == -1) face->layer = 0;

			if ((face->use == 1) && (bdry_use1 == 0)) bdry_use1 = face;
			else if ((face->use == 2) && (bdry_use2 == 0)) bdry_use2 = face;
			else if ((face->use == -1) && (bdry_useNeg == 0)) bdry_useNeg = face;*/

/*//debug markos
			if (((face->verts[0] == 401) && (face->verts[1] == 433) && (face->verts[2] == 578)) ||
				((face->verts[0] == 433) && (face->verts[1] == 401) && (face->verts[2] == 465)) ||
				((face->verts[0] == 465) && (face->verts[1] == 401) && (face->verts[2] == 489)) ||
				((face->verts[0] == 433) && (face->verts[1] == 465) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 401) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 578) && (face->verts[2] == 465)))
			{
				fprintf(stdout, "debug markos, setting face->use = %d for face %d, %d, %d\n", face->use, face->verts[0], face->verts[1], face->verts[2]);
			}
//end debug markos*/

			return 0;
		}
	}

	return 1;
}

static int PMsh3DBoundaryFace(Msh3DBdryFace face)
{
	if (face->layer == 0)
	{
		return 1;
	}

	return 0;
}

static Msh3DBdryFace PMsh3DRemainingFrontFace(int v0, int v1, int v2)
{
	int check = 0, k = 0;

	/* check if face is on the front remaining from advancing */
	Msh3DBdryFace curr = bdry_stack;

	while (curr)
	{
		if (curr->use == -1)
		{
			check = 0;

			for (k = 0; k < 3; k++)
			{
				if ((curr->verts[k] == v0) ||
					(curr->verts[k] == v1) ||
					(curr->verts[k] == v2))
				{
					check++ ;
				}
			}

			if (check == 3)
			{
				return curr;
			}
		}

		curr = curr->next;
	}

	return 0;
}

static int PMsh3DIsRemainingFrontFace(int v0, int v1, int v2)
{
	return (PMsh3DRemainingFrontFace(v0, v1, v2) != 0);
}

static void PMsh3DInvert(Msh3DBdryFace face)
{
	int v1, v2, v3, i;
	double tmp;

	v1 = face->verts[1];
	v2 = face->verts[0];
	v3 = face->verts[2];

	face->verts[0] = v1;
	face->verts[1] = v2;
	face->verts[2] = v3;

    for (i = 0; i < 3; i++)
    {
        face->r[i] = -face->r[i];

        tmp = face->s[i];
        face->s[i] = face->t[i];
        face->t[i] = tmp;

        face->r_x_s[i] = -face->r_x_s[i];
    }

    face->rs_dot_v1 = -face->rs_dot_v1;
}

static int PMsh3DIsInvertedFace(Msh3DBdryFace face)
{
	struct PMsh3DInvertedFace *inv_face = pmsh3d_inverted;

	while (inv_face)
	{
		if (inv_face->face == face)
		{
			return 1;
		}

		inv_face = inv_face->next;
	}

	return 0;
}

static int PMsh3DInvertFace(Msh3DBdryFace face)
{
	PMsh3DInvert(face);

	struct PMsh3DInvertedFace *inv_face = pmsh3d_inverted;
	struct PMsh3DInvertedFace *inv_prev = 0;

	while (inv_face)
	{
		if (inv_face->face == face)
		{
			break;
		}

		inv_prev = inv_face;
		inv_face = inv_face->next;
	}

	if (inv_face)
	{
		if (inv_prev)
		{
			inv_prev->next = inv_face->next;
		}
		else
		{
			pmsh3d_inverted = inv_face->next;
		}

		face->use = -1;

		Msh3DFree(inv_face);

		return 0;
	}

	inv_face = (struct PMsh3DInvertedFace *)Msh3DMalloc(sizeof(struct PMsh3DInvertedFace));

	inv_face->face = face;
	inv_face->next = pmsh3d_inverted;

	pmsh3d_inverted = inv_face;

	return 1;
}

static void PMsh3DInvertFaces()
{
	//struct PMsh3DInvertedFace *curr = 0;

	while (pmsh3d_inverted)
	{
		PMsh3DInvertFace(pmsh3d_inverted->face);

		/*curr = pmsh3d_inverted;

		pmsh3d_inverted = pmsh3d_inverted->next;

		PMsh3DInvert(curr->face);

		curr->face->use = -1;

		Msh3DFree(curr);*/
	}
}

static void PMsh3DPrintBdryList(char *s, Msh3DBdryFace face, int print, int printBdry, int printPolyStack)
{
	if (s)
	{
		fprintf(stdout, "%s", s);
	}

	if (face)
	{
		fprintf(stdout, "face in operation = %p\n", face);
		fprintf(stdout, "face->prev = %p\n", face->prev);
		fprintf(stdout, "face->use = %d\n", face->use);
		fprintf(stdout, "face->verts = %d %d %d\n", face->verts[0], face->verts[1], face->verts[2]);
		fprintf(stdout, "face->next = %p\n", face->next);
	}

	if (print)
	{
		fprintf(stdout, "start printing\n");
	}

	Msh3DBdryFace curr = bdry_stack;

	int count = 0;

	while (curr)
	{
		curr = curr->next;

		count++;
	}

	fprintf(stdout, "num_faces in bdry = %d\n", count);

	if (!print)
	{
		return;
	}

	if (bdry_stack) fprintf(stdout, "bdry_stack->prev = %p\n", bdry_stack->prev);
	fprintf(stdout, "bdry_stack = %p\n", bdry_stack);
	if (bdry_stack) fprintf(stdout, "bdry_stack->next = %p\n", bdry_stack->next);

	if (printBdry)
	{
		curr = bdry_stack;

		while (curr)
		{
			fprintf(stdout, "curr->prev = %p\n", curr->prev);
			fprintf(stdout, "curr = %p\n", curr);
			fprintf(stdout, "curr->use = %d\n", curr->use);
			fprintf(stdout, "curr in boundary = %d\n", Msh3DBoundaryFace(curr, curr->verts));
			fprintf(stdout, "curr->verts = %d %d %d\n", curr->verts[0], curr->verts[1], curr->verts[2]);
			fprintf(stdout, "curr->verts[0]->coords = %lf %lf %lf\n", node_list[curr->verts[0]].coord[0], node_list[curr->verts[0]].coord[1], node_list[curr->verts[0]].coord[2]);
			fprintf(stdout, "curr->verts[1]->coords = %lf %lf %lf\n", node_list[curr->verts[1]].coord[0], node_list[curr->verts[1]].coord[1], node_list[curr->verts[1]].coord[2]);
			fprintf(stdout, "curr->verts[2]->coords = %lf %lf %lf\n", node_list[curr->verts[2]].coord[0], node_list[curr->verts[2]].coord[1], node_list[curr->verts[2]].coord[2]);
			fprintf(stdout, "curr->next = %p\n", curr->next);

			curr = curr->next;
		}
	}

	if (bdry_last) fprintf(stdout, "bdry_last->prev = %p\n", bdry_last->prev);
	fprintf(stdout, "bdry_last = %p\n", bdry_last);
	if (bdry_last) fprintf(stdout, "bdry_last->next = %p\n", bdry_last->next);

	if (bdry_try) fprintf(stdout, "bdry_try->prev = %p\n", bdry_try->prev);
	fprintf(stdout, "bdry_try = %p\n", bdry_try);
	if (bdry_try) fprintf(stdout, "bdry_try->next = %p\n", bdry_try->next);

	if (bdry_use1) fprintf(stdout, "bdry_use1->prev = %p\n", bdry_use1->prev);
	fprintf(stdout, "bdry_use1 = %p\n", bdry_use1);
	if (bdry_use1) fprintf(stdout, "bdry_use1->next = %p\n", bdry_use1->next);

	if (bdry_use2) fprintf(stdout, "bdry_use2->prev = %p\n", bdry_use2->prev);
	fprintf(stdout, "bdry_use2 = %p\n", bdry_use2);
	if (bdry_use2) fprintf(stdout, "bdry_use2->next = %p\n", bdry_use2->next);

	if (bdry_useNeg) fprintf(stdout, "bdry_useNeg->prev = %p\n", bdry_useNeg->prev);
	fprintf(stdout, "bdry_useNeg = %p\n", bdry_useNeg);
	if (bdry_useNeg) fprintf(stdout, "bdry_useNeg->next = %p\n", bdry_useNeg->next);

	if (bdry_tail) fprintf(stdout, "bdry_tail->prev = %p\n", bdry_tail->prev);
	fprintf(stdout, "bdry_tail = %p\n", bdry_tail);
	if (bdry_tail) fprintf(stdout, "bdry_tail->next = %p\n", bdry_tail->next);

	if (printPolyStack)
	{
		Msh3DPolyFace curr_poly = poly_stack;

		while (curr_poly)
		{
			fprintf(stdout, "poly->prev = %p\n", curr_poly->prev);
			fprintf(stdout, "poly->face = %p\n", curr_poly->face);
			fprintf(stdout, "poly->face->use = %d\n", curr_poly->face->use);
			fprintf(stdout, "poly->face in boundary = %d\n", Msh3DBoundaryFace(curr_poly->face, curr_poly->face->verts));
			fprintf(stdout, "poly->face->verts = %d %d %d\n", curr_poly->face->verts[0], curr_poly->face->verts[1], curr_poly->face->verts[2]);
			fprintf(stdout, "poly->face->verts[0]->coords = %lf %lf %lf\n", node_list[curr_poly->face->verts[0]].coord[0], node_list[curr_poly->face->verts[0]].coord[1], node_list[curr_poly->face->verts[0]].coord[2]);
			fprintf(stdout, "poly->face->verts[1]->coords = %lf %lf %lf\n", node_list[curr_poly->face->verts[1]].coord[0], node_list[curr_poly->face->verts[1]].coord[1], node_list[curr_poly->face->verts[1]].coord[2]);
			fprintf(stdout, "poly->face->verts[2]->coords = %lf %lf %lf\n", node_list[curr_poly->face->verts[2]].coord[0], node_list[curr_poly->face->verts[2]].coord[1], node_list[curr_poly->face->verts[2]].coord[2]);
			fprintf(stdout, "poly->next = %p\n", curr_poly->next);

			curr_poly = curr_poly->next;
		}
	}

	fprintf(stdout, "\n\n");
}

void PMsh3DCheckBdryUse()
{
	Msh3DBdryFace face;

	Msh3DBdryReset();

	while ((face = Msh3DBdryNext()))
	{
		if (face->use != -1)
		{
			fprintf(stdout, "face (%d, %d, %d) has use = %d\n", face->verts[0], face->verts[1], face->verts[2], face->use);
		}
	}
}
//end added by Markos

/* -------------------------------------------------------------
** Msh3DBdryContraction - main driver for the boundary contraction
**                        algorithm.
*/

int PMsh3DBdryContraction(
 int    version,
 int    offset,
 int    num_org_nodes,
 int    num_org_faces,
 double **original_nodes,
 int    *toriginal_nodes,
 int    **original_faces,
 int    *num_int_nodes,
 double **internal_nodes,
 int    **tinternal_nodes,
 int    *num_gen_elements,
 int    **generated_elements,
/* PJmesh parameters */
 int    pjmesh_num_original_elements,
 int    pjmesh_num_original_faces,
 int    *pjmesh_original_faces,
 int    pjmesh_use_box_test,
 double *pjmesh_box_min,
 double *pjmesh_box_max,
 double pjmesh_range_proportion)
{
    Msh3DBdryFace     face ;
    Msh3DBdryFace     cur ;
    Msh3DBdryFace     next ;
    Msh3DBdryNode     node ;
    Msh3DAdjFace      adj_face ;
    Msh3DAdjIniFace   adj_ini_face ;
    Msh3DBdryFace     bface ;
    int               num_nodes, bdr_nodes, cur_nodes, all_nodes = 0 ;
    int               node_indx, next_indx ;
    int               node_bdry ;
    double            metric, delta, next_metric ;
    int               check ;
    int               valid ;
    int               alloc ;
    int               status ;
    int               shape = 0, step = 0 ;
    double            cand_vec[3], dot, tol, tol_mate, new[3] ;
    double            len, size = 1.0e+7 ;
    int               i, j, k, n, l, trials, face_pop = 0 ;
    int               face_done = 0, face_number = 10 ;
    int               face_draw = 0, face_try = 0 ;
    int               v1i, v2i, v1, v2, found, exist, ind, nind ;
    int               *aux_vec_nodes, aux_num_nodes, nodeid ;
    int               *aux_vec_elems, aux_num_elems ;
    int               numcds, curol, curfa, print ;
    PointCoord        *cds, pt ;
    int               *ids, dur, id, lnum, fur, *fid ;
    double            d, did, dist, dist_vec[3], factor, dnew[3] ;
    int               *point_result, point_size, m ;
    PointCoord        imin, imax ;
    RangeCoord        *min, *max ;
    int               *range_result, range_size, node_id[3] ;
    RangeCoord        minc, maxc ;
    double            minf[3], maxf[3], l1, l2, l3 ;
    int               cur_gen_elements ;

    double            pjmesh_pt[3];
    double            pjmesh_r[3], pjmesh_s[3], pjmesh_r_x_s[3];
    int               pjmesh_out_pt/*, pjmesh_count = 0*/;
    int               pjmesh_v1, pjmesh_v2, pjmesh_v3;
    PointCoord        pjmesh_imin, pjmesh_imax;
    int               pjmesh_force_back_track = 0;
    int               pjmesh_redefine_global_box = 0;

/* 2.0 Set jmesh version */

   jmesh_version = version ;

/* 2.1 Build the node list, then push all the boundary face
       descriptions onto a stack  */

    fprintf( stderr, "\n" );
    fprintf (stderr, "Front Initialization:") ;
    fprintf( stderr, "\n" );

#if DSP_TIME
    init_time = clock( );
    fprintf( stderr, "\n" );
    fprintf( stderr, "	Generating initial front.............." );
    fprintf( stderr, "\n" );
#endif

#if VER_BOTH
    all_nodes = shape = step = 0 ;
    face_pop = face_done = 0 ;
    face_draw = face_try = 0 ;
    face_number = 10 ;
    size = 1.0e+7 ;
    *num_gen_elements = 0 ;
    num_elem_alloced  = 0 ;
    num_elem_studied  = 0 ;
    int_offset = offset ;
    bdry_last  = 0 ;
    bdry_use1  = 0 ;
    bdry_use2  = 0 ;
    bdry_useNeg  = 0 ;
#else
    *num_gen_elements = 0 ;
    num_elem_alloced  = 0 ;
    int_offset = offset ;
#endif

	/* PJmesh - some elements can be given as input */
	if (pjmesh_num_original_elements != 0)
	{
		*num_gen_elements = num_elem_alloced = num_elem_studied = pjmesh_num_original_elements;

		if ((gmin[0] == 0.0) && (gmin[1] == 0.0) && (gmin[2] == 0.0) &&
            (gmax[0] == 0.0) && (gmax[1] == 0.0) && (gmax[2] == 0.0))
        {
            pjmesh_redefine_global_box = 1;
        }
	}
	/* end PJmesh - some elements can be given as input */

    nintnode   = (*num_int_nodes) ;
    nbdrynode  = num_org_nodes ;
    num_nodes  = num_org_nodes + (*num_int_nodes) ;
    node_alloc = num_nodes ;
    node_bdry  = num_nodes ;
    node_list  = (Msh3DBdryNodeList)Msh3DMalloc( num_nodes *
                              sizeof(Msh3DBdryNodeRec) ) ;

    /* Build the node list and the initial PT TREE */

#if defined(PT_TREE) || defined(PT_DEBU)
    numcds = num_org_nodes + (*num_int_nodes) ;
    cds = (PointCoord *)Msh3DCalloc(numcds,sizeof(PointCoord)) ;
    ids = (int *)Msh3DCalloc(numcds,sizeof(int)) ;
    dur = 0 ;
    for ( i=0 ; i<num_org_nodes ; i++ ) {
        if( toriginal_nodes != NULL )
         node_list[i].real_id = toriginal_nodes[i] ;
        else
         node_list[i].real_id = i ;
        for ( j=0 ; j<3 ; j++ ) {
          node_list[i].coord[j] = original_nodes[i][j] ;
          cds[dur][j] = node_list[i].coord[j] ;

          /* PJmesh - global box not defined */
          if (pjmesh_redefine_global_box)
          {
              if ((i == 0) || (node_list[i].coord[j] < gmin[j]))
              {
                  gmin[j] = node_list[i].coord[j];
              }

              if ((i == 0) || (node_list[i].coord[j] > gmax[j]))
              {
                  gmax[j] = node_list[i].coord[j];
              }
          }
          /* end PJmesh - global box not defined */
        }
	ids[dur] = i ;
	++dur ;
        node_list[i].active_flag = 1 ;
        node_list[i].faces  = (Msh3DAdjFaceList)0 ;
        node_list[i].ifaces = (Msh3DAdjIniFaceList)0 ;
	node_list[i].mates[0] = 0 ;
    }
    for ( i=num_org_nodes,k=0 ; k<(*num_int_nodes) ; i++,k++ ) {
        node_list[i].real_id = (*tinternal_nodes)[i] ;
        for ( j=0 ; j<3 ; j++ ) {
          node_list[i].coord[j] = (*internal_nodes)[k*3+j] ;
	  cds[dur][j] = node_list[i].coord[j] ;

          /* PJmesh - global box not defined */
          if (pjmesh_redefine_global_box)
          {
              if ((i == 0) || (node_list[i].coord[j] < gmin[j]))
              {
                  gmin[j] = node_list[i].coord[j];
              }

              if ((i == 0) || (node_list[i].coord[j] > gmax[j]))
              {
                  gmax[j] = node_list[i].coord[j];
              }
          }
          /* end PJmesh - global box not defined */
        }
	ids[dur] = i ;
	++dur ;
        node_list[i].active_flag = 2 ;
        node_list[i].faces  = (Msh3DAdjFaceList)0 ;
        node_list[i].ifaces = (Msh3DAdjIniFaceList)0 ;
	node_list[i].mates[0] = 0 ;
    }
    InitPointTree (dur, cds, ids) ;
    Msh3DFree (cds) ;
    Msh3DFree (ids) ;
#else
    for ( i=0 ; i<num_org_nodes ; i++ ) {
        if( toriginal_nodes != NULL )
         node_list[i].real_id = toriginal_nodes[i] ;
        else
         node_list[i].real_id = i ;
        for ( j=0 ; j<3 ; j++ )
        {
          node_list[i].coord[j] = original_nodes[i][j] ;

          /* PJmesh - global box not defined */
          if (pjmesh_redefine_global_box)
          {
              if ((i == 0) || (node_list[i].coord[j] < gmin[j]))
              {
                  gmin[j] = node_list[i].coord[j];
              }

              if ((i == 0) || (node_list[i].coord[j] > gmax[j]))
              {
                  gmax[j] = node_list[i].coord[j];
              }
          }
          /* end PJmesh - global box not defined */
        }
        node_list[i].active_flag = 1 ;
        node_list[i].faces  = (Msh3DAdjFaceList)0 ;
        node_list[i].ifaces = (Msh3DAdjIniFaceList)0 ;
	node_list[i].mates[0] = 0 ;
    }
    for ( i=num_org_nodes,k=0 ; k<(*num_int_nodes) ; i++,k++ ) {
        node_list[i].real_id = (*tinternal_nodes)[i] ;
        for ( j=0 ; j<3 ; j++ )
        {
          node_list[i].coord[j] = (*internal_nodes)[k*3+j] ;

          /* PJmesh - global box not defined */
          if (pjmesh_redefine_global_box)
          {
              if ((i == 0) || (node_list[i].coord[j] < gmin[j]))
              {
                  gmin[j] = node_list[i].coord[j];
              }

              if ((i == 0) || (node_list[i].coord[j] > gmax[j]))
              {
                  gmax[j] = node_list[i].coord[j];
              }
          }
          /* end PJmesh - global box not defined */
        }
        node_list[i].active_flag = 2 ;
        node_list[i].faces  = (Msh3DAdjFaceList)0 ;
        node_list[i].ifaces = (Msh3DAdjIniFaceList)0 ;
	node_list[i].mates[0] = 0 ;
    }
#endif

    /* Build the face list and the initial AD Tree. For the initial
     * boundary, even if the version is the ADVAN_VERSION, all faces
     * on the original boundary should be included by SMALL_VERSION.
     * This is important for the advancing-front process to be made. */

    push_version = SMALL_VERSION ;
    startid = 0 ;

#if defined(AD_TREE) || defined(AD_DEBU)
    /* PJmesh - num_org_faces might be 0 */
    if (num_org_faces > 0)
	{
    adtreesize = num_org_faces ;
    adtreeindx = 0 ;
    adtreehash=(Msh3DBdryFace *)Msh3DCalloc(adtreesize,sizeof(Msh3DBdryFace)) ;
    min = (RangeCoord *) Msh3DCalloc (adtreesize,sizeof(RangeCoord)) ;
    max = (RangeCoord *) Msh3DCalloc (adtreesize,sizeof(RangeCoord)) ;
    fid = (int *) Msh3DCalloc (adtreesize, sizeof (int)) ;
    for ( i=0 ; i<num_org_faces ; i++ ) {
        face = Msh3DPushBdryFace( node_list,
                  original_faces[i][0],original_faces[i][1],
                  original_faces[i][2], 0, 0 ) ;
	face->adtreeindx = adtreeindx ;
	min[adtreeindx][0] = face->min[0] ;
	min[adtreeindx][1] = face->min[1] ;
	min[adtreeindx][2] = face->min[2] ;
	max[adtreeindx][0] = face->max[0] ;
	max[adtreeindx][1] = face->max[1] ;
	max[adtreeindx][2] = face->max[2] ;
	fid[adtreeindx] = adtreeindx ;
	adtreehash[adtreeindx] = face ;
	adtreeindx++ ;
    }
    InitRangeTree (num_org_faces, min, max, fid) ;
    Msh3DFree (min) ;
    Msh3DFree (max) ;
    Msh3DFree (fid) ;
	}
#else
    for ( i=0 ; i<num_org_faces ; i++ ) {
        face = Msh3DPushBdryFace( node_list,
                  original_faces[i][0],original_faces[i][1],
                  original_faces[i][2], 0, 0 ) ;
    }
#endif

	/* Retrieve the original version to insert faces from now on */

    push_version = jmesh_version ;

    /* get the smallest area in the model */

    /* PJmesh - num_org_faces might be 0 */
    if (num_org_faces > 0)
		smallest_area = bdry_stack->area;

    /* get the last face (biggest face) in the original boundary if it's
     * the small version. This is done because new faces will be inserted
     * in order of size but only after the last face on the original
     * boundary. This also helps the advancing-front process to be made. */

    if ( jmesh_version == SMALL_VERSION )
      bdry_last = bdry_tail ;
    else
      bdry_last = 0 ;

    /* build the list of adjacent faces and initial adjacent faces
       for all the boundary nodes. Also find the smallest edge of
       the model, that will be used to define tolerance. Finally,
       find all mate nodes for the boundary nodes. The first position
       on the mates vector (mates[0]) is the number of mate nodes. */

    Msh3DBdryReset() ;
    while ( face = Msh3DBdryNext() ) {
	tol_mate = sqrt(face->area) * 1.0e-8 ;
        for ( j=0 ; j<3 ; j++ ) {
            /* build adjacent lists */
            node = &node_list[face->verts[j]] ;
            adj_face     = Msh3DAdjFaceAlloc() ;
            adj_ini_face = Msh3DAdjIniFaceAlloc() ;
            adj_face->next     = node->faces ;
            adj_ini_face->next = node->ifaces ;
            adj_face->face = face ;
            for ( k=0; k<3; k++ ) {
             adj_ini_face->verts[k] = face->verts[k] ;
             adj_ini_face->r_x_s[k] = face->r_x_s[k] ;
            }
            node->ifaces = adj_ini_face ;
            node->faces  = adj_face ;
	    /* find the smallest edge */
            cand_vec[0] = node_list[face->verts[j]].coord[0] -
                          node_list[face->verts[(j+1)%3]].coord[0] ;
            cand_vec[1] = node_list[face->verts[j]].coord[1] -
                          node_list[face->verts[(j+1)%3]].coord[1] ;
            cand_vec[2] = node_list[face->verts[j]].coord[2] -
                          node_list[face->verts[(j+1)%3]].coord[2] ;
            len = sqrt ( cand_vec[0] * cand_vec[0] +
                         cand_vec[1] * cand_vec[1] +
                         cand_vec[2] * cand_vec[2] ) ;
            if( len < size ) size = len ;
	    /* find mate nodes for all nodes, if any (case of cracks) */
#if OLD_INTE
	    node_list[face->verts[j]].mates[0] = 0 ;
#else
            if (node_list[face->verts[j]].mates[0] == 0) {
	     /* if we get here is becuse this node has not been visited
	      * yet. So, we then loop through all nodes in the model and
	      * see if they are located within the tolerance distance */
             for (i = 0; i < num_nodes; i++) {
	      if (i == face->verts[j]) continue ;
               cand_vec[0] = node_list[face->verts[j]].coord[0] -
                             node_list[i].coord[0] ;
               cand_vec[1] = node_list[face->verts[j]].coord[1] -
                             node_list[i].coord[1] ;
               cand_vec[2] = node_list[face->verts[j]].coord[2] -
                             node_list[i].coord[2] ;
               len = sqrt ( cand_vec[0] * cand_vec[0] +
                            cand_vec[1] * cand_vec[1] +
                            cand_vec[2] * cand_vec[2] ) ;
               if( len < tol_mate ) {
	        /* then nodes are within tolerance so
	         * insert each into each nodes list of mates */
	        node_list[face->verts[j]].mates[0] += 1 ;
	        k = node_list[face->verts[j]].mates[0] ;
	        node_list[face->verts[j]].mates[k] = i ;
	        node_list[i].mates[0] += 1 ;
	        k = node_list[i].mates[0] ;
	        node_list[i].mates[k] = face->verts[j] ;
               }
	     }
	    }
#endif
        }
    }

    /* PJmesh - setting internal nodes as inactives */
	if (pjmesh_num_original_elements != 0)
	{
		Msh3DAdjIniFreeAll();

		for (i = 0; i < num_nodes; i++)
		{
			if (!node_list[i].faces)
			{
				node_list[i].active_flag = 0;

#if PT_TREE
				pjmesh_pt[0] = node_list[i].coord[0];
				pjmesh_pt[1] = node_list[i].coord[1];
				pjmesh_pt[2] = node_list[i].coord[2];

				RemoveFromPointTree(pjmesh_pt, i);
#endif //#if PT_TREE
			}
			else
			{
				node_list[i].active_flag = 1;

				node_list[i].ifaces = (Msh3DAdjIniFaceList)0;
			}
		}

		for (i = 0; i < pjmesh_num_original_faces; i++)
		{
			pjmesh_v1 = pjmesh_original_faces[3*i + 0];
			pjmesh_v2 = pjmesh_original_faces[3*i + 1];
			pjmesh_v3 = pjmesh_original_faces[3*i + 2];

			for (k = 0; k < 3; k++)
			{
				pjmesh_r[k] = node_list[pjmesh_v2].coord[k] - node_list[pjmesh_v1].coord[k];
				pjmesh_s[k] = node_list[pjmesh_v3].coord[k] - node_list[pjmesh_v1].coord[k];
			}

			pjmesh_r_x_s[0] = pjmesh_r[1] * pjmesh_s[2] - pjmesh_s[1] * pjmesh_r[2] ;
			pjmesh_r_x_s[1] = pjmesh_r[2] * pjmesh_s[0] - pjmesh_s[2] * pjmesh_r[0] ;
			pjmesh_r_x_s[2] = pjmesh_r[0] * pjmesh_s[1] - pjmesh_s[0] * pjmesh_r[1] ;

			for (j = 0; j < 3; j++)
			{
				node = &node_list[pjmesh_original_faces[3*i + j]];

				adj_ini_face = Msh3DAdjIniFaceAlloc();

				adj_ini_face->next = node->ifaces;

				for (k = 0; k < 3; k++)
				{
					adj_ini_face->verts[k] = pjmesh_original_faces[3*i + k];
					adj_ini_face->r_x_s[k] = pjmesh_r_x_s[k] ;
				}

				node->ifaces = adj_ini_face;
			}
		}
	}
	/* end PJmesh - setting internal nodes as inactives */

    /* define tolerance based on the smallest edge on the model */

#if OLD_TOLE
    tol_inters = TOL_INTERS * size ;
#else
    tol_inters = 1.0e-08 * size ;
#endif

	/*fprintf(stdout, "debug markos, tol_mate = %0.20f\n", tol_mate);
    fprintf(stdout, "debug markos, tol = %0.20f\n", tol);
    fprintf(stdout, "debug markos, tol_factor = %0.20f\n", tol_factor);
    fprintf(stdout, "debug markos, tol_inters = %0.20f\n", tol_inters);*/

    /* print feedback messages */

#if DSP_TIME
    fprintf( stderr,"\n") ;
    tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
    fprintf( stderr,"\t\t Initial front size: %8d fac.\n", num_org_faces);
    fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.", (double)tot_time);
    fprintf( stderr,"\n") ;
#endif

    fprintf( stderr,"\n") ;
    fprintf (stderr,"Boundary Contraction:") ;
    fprintf( stderr,"\n") ;

#if DSP_MESH
    if ( FDEBUG ) {
      GdbIni ("Mesh3d Generator - Advancing Front Technique");
      GdbDrawBdry () ;
    }
#endif

/* 2.2 Use the face on the top of the stack.  If the stack is
       empty then we are done */

   /*//debug markos
   PMshCountPartition++;
   //endebug markos*/

    while ( face = Msh3DBdryPop() ) {

		/*//debug markos
		if (PMshCountPartition == PMshStudiedPartition)
		{
			static int count_elems = -1;

			if ((count_elems == -1) && (pjmesh_num_original_elements != 0))
			{
				count_elems += pjmesh_num_original_elements;
			}

			if (*num_gen_elements != count_elems)
			{
				PMsh3DPrintBdryList("\nwill now work to advance\n", face, 1, 1, 0);

				count_elems = *num_gen_elements;
			}
		}
		//endebug markos*/

    /* update bdry_last because after all the faces on the original
     * boundary were used, from now on we should always pop the
     * smallest face in the current front. This helps the process. */

    if ( jmesh_version == SMALL_VERSION ) {
      if (face == bdry_last)
        bdry_last = 0 ;
    }

/*//debug markos
	if (((face->verts[0] == 96) && (face->verts[1] == 111) && (face->verts[2] == 100)) ||
		((face->verts[0] == 433) && (face->verts[1] == 401) && (face->verts[2] == 465)) ||
		((face->verts[0] == 465) && (face->verts[1] == 401) && (face->verts[2] == 489)) ||
		((face->verts[0] == 433) && (face->verts[1] == 465) && (face->verts[2] == 578)) ||
		((face->verts[0] == 489) && (face->verts[1] == 401) && (face->verts[2] == 578)) ||
		((face->verts[0] == 489) && (face->verts[1] == 578) && (face->verts[2] == 465)) ||
		false)
	{
		fprintf(stdout, "debug markos, found face %d, %d, %d, use = %d\n", face->verts[0], face->verts[1], face->verts[2], face->use);
	}
//end debug markos*/

    /* print each step we are. Also rebuild both trees in the
     * beginning of the topology-based phase, to balance better
     * the trees and speed up the process. It is not necessary
     * to do this in the geometry-based phase because the trees
     * were originally created for this phase and also for the
     * back-tracking phase because in this phase all operations
     * are local, and don't need the trees to speed them up. */

    if (face->use == 0) {
		if (face_try == 0) {
#if DSP_TIME
      init_time = clock( );
      num_elem_studied += 500;
      fprintf( stderr, "\n" );
      fprintf( stderr, "	Generating geometry-based elems.......\n" );
      fprintf( stderr, "\n" );
#endif
      if (face_pop) {
       face_pop = 0 ;
      }
      face_try++ ;
     }
#if DSP_TIME
     if (*num_gen_elements >= num_elem_studied)
     {
      tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
      fprintf( stderr,"\n") ;
      init_time = clock( );
      num_elem_studied += 500;
     }
#endif
    }

    if (face->use == 1) {
     if (face_try == 1) {
#if DSP_TIME
      tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
      fprintf( stderr,"\n") ;
#endif
      if (face_pop) {
        face_pop = 0 ;
       }
      face_try++ ;
#if DSP_TIME
      init_time = clock( );
      if (*num_gen_elements >= num_elem_studied)
       num_elem_studied += 100;
      fprintf( stderr, "	Generating topology-based elems.......\n" );
      fprintf( stderr,"\n") ;
#endif
#if defined(AD_TREE) || defined(AD_DEBU)
      lnum = 0 ;
      fur = 0 ;
      fprintf( stderr,"\t\t Rebuilding range tree:\n") ;
      CleanRangeTree( ) ;
      Msh3DBdryReset() ;
      while (bface = Msh3DBdryNext ()) lnum++ ;
      //added by markos
      lnum++; //to account for the current face that will be advanced
      //end added by markos
      min = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
      max = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
      fid = (int *)Msh3DCalloc(lnum, sizeof(int)) ;
      Msh3DBdryReset() ;
      while (bface = Msh3DBdryNext ()) {
       min[fur][0] = bface->min[0] ;
       min[fur][1] = bface->min[1] ;
       min[fur][2] = bface->min[2] ;
       max[fur][0] = bface->max[0] ;
       max[fur][1] = bface->max[1] ;
       max[fur][2] = bface->max[2] ;
       fid[fur] = bface->adtreeindx ;
       fur++ ;
      }
      //added by markos
      {
       bface = face;
       min[fur][0] = bface->min[0] ;
       min[fur][1] = bface->min[1] ;
       min[fur][2] = bface->min[2] ;
       max[fur][0] = bface->max[0] ;
       max[fur][1] = bface->max[1] ;
       max[fur][2] = bface->max[2] ;
       fid[fur] = bface->adtreeindx ;
       fur++ ;
      }
      //end added by markos
#if DSP_TIME
      init_tree_time = clock( );
#endif
      InitRangeTree (fur,min,max,fid) ;
#if DSP_TIME
      tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Size of range tree: %8d fac.\n", fur) ;
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
      fprintf( stderr,"\n") ;
#endif
      Msh3DFree (min) ;
      Msh3DFree (max) ;
      Msh3DFree (fid) ;
#endif
#if defined(PT_TREE) || defined(PT_DEBU)
      dur = 0 ;
      fprintf( stderr,"\t\t Rebuilding point tree:\n") ;
      CleanPointTree( ) ;
      ids = (int *)Msh3DCalloc(num_nodes,sizeof(int));
      cds = (PointCoord *)Msh3DCalloc(num_nodes,sizeof(PointCoord));
      for ( i=0 ; i<num_nodes ; i++ )
      {
       if ( node_list[i].active_flag ) {
        ids[dur] = i ;
        cds[dur][0] = node_list[i].coord[0] ;
        cds[dur][1] = node_list[i].coord[1] ;
        cds[dur][2] = node_list[i].coord[2] ;
        ++dur ;
       }
      }
#if DSP_TIME
      init_tree_time = clock( );
#endif
      InitPointTree (dur,cds,ids) ;
#if DSP_TIME
      tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Size of point tree: %8d nod.\n", dur) ;
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
      fprintf( stderr,"\n") ;
#endif
      Msh3DFree (cds) ;
      Msh3DFree (ids) ;
#endif
     }
#if DSP_TIME
     if (*num_gen_elements >= num_elem_studied)
     {
      tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
      fprintf( stderr,"\n") ;
      init_time = clock( );
      num_elem_studied += 100;
     }
#endif
    }

    if (face->use == 2) {
     if (face_try == 2) {
#if DSP_TIME
      tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
      fprintf( stderr,"\n") ;
#endif
      if (face_pop) {
       face_pop = 0 ;
      }
      face_try++ ;
#if DSP_TIME
      init_time = clock( );
      if (*num_gen_elements >= num_elem_studied)
       num_elem_studied += 100;
      fprintf( stderr, "	Generating back-tracking elems........\n" );
      fprintf( stderr, "\n" );
#endif
     }
#if DSP_TIME
     if (*num_gen_elements >= num_elem_studied)
     {
      tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
      fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
      fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
      fprintf( stderr,"\n") ;
      init_time = clock( );
      num_elem_studied += 100;
     }
#endif
    }

//fprintf (stderr,"markos debug, face = %d, %d, %d\n", face->verts[0], face->verts[1], face->verts[2]) ;
//fprintf (stderr,"markos debug, face->v1 = %f, %f, %f\n", node_list[face->verts[0]].coord[0], node_list[face->verts[0]].coord[1], node_list[face->verts[0]].coord[2]) ;
//fprintf (stderr,"markos debug, face->v2 = %f, %f, %f\n", node_list[face->verts[1]].coord[0], node_list[face->verts[1]].coord[1], node_list[face->verts[1]].coord[2]) ;
//fprintf (stderr,"markos debug, face->v3 = %f, %f, %f\n", node_list[face->verts[2]].coord[0], node_list[face->verts[2]].coord[1], node_list[face->verts[2]].coord[2]) ;
//fprintf (stderr,"markos debug, mid  = %f, %f, %f\n", face->center[0], face->center[1], face->center[2]) ;
//fprintf (stderr,"markos debug, area = %f\n", face->area) ;

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
    GdbReset();
    GdbSetCurFaceOn(face);
    GdbClear();
  }
#endif

#if DSP_DRAW
  if ( FDEBUG && !GdbWait ) {
   if( face_draw > Msh3D_BDRY_FACE_DRAW ) {
    GdbClear();
    GdbReset();
    GdbSetCurFaceOn(face);
    GdbDrawBdry ();
    face_draw = 0 ;
   }
  }
#endif

/* PJMesh - check if the face has all nodes inside the bounding box */
	if (pjmesh_use_box_test)
	{
		pjmesh_out_pt = 0;
		//pjmesh_count++;

		if (face->use == 0)
		{
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					pjmesh_pt[j] = node_list[face->verts[i]].coord[j];
				}

				if (PMsh3DOutBoxPt(pjmesh_pt, pjmesh_box_min, pjmesh_box_max))
				{
					pjmesh_out_pt = 1;

					break;
				}
			}
		}

		if (pjmesh_out_pt)
		{
			face->use = -1;
			if (face->use == -1) face->layer = 0;

			if ((face->use == 1) && (bdry_use1 == 0)) bdry_use1 = face;
			else if ((face->use == 2) && (bdry_use2 == 0)) bdry_use2 = face;
			else if ((face->use == -1) && (bdry_useNeg == 0)) bdry_useNeg = face;

/*//debug markos
			if (((face->verts[0] == 401) && (face->verts[1] == 433) && (face->verts[2] == 578)) ||
				((face->verts[0] == 433) && (face->verts[1] == 401) && (face->verts[2] == 465)) ||
				((face->verts[0] == 465) && (face->verts[1] == 401) && (face->verts[2] == 489)) ||
				((face->verts[0] == 433) && (face->verts[1] == 465) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 401) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 578) && (face->verts[2] == 465)))
			{
				fprintf(stdout, "debug markos, setting face->use = %d for face %d, %d, %d\n", face->use, face->verts[0], face->verts[1], face->verts[2]);
			}
//end debug markos*/

			/*fprintf (stderr,"markos debug, face %d has point out of bounding box\n", pjmesh_count - 1);*/

			Msh3DBdryPush(face);

			continue;
		}
	}
/* end PJMesh - check if the face has all nodes inside the bounding box */

/* 2.3 Examine all active nodes. Rank all nodes as to the goodness
       of the tetrahedral they will form with the current face */

#if PT_TREE
       /* If we are in the ideal size, use PT Tree for search,
	* otherwise, check for all nodes in other phases */

        Msh3DTreeNode (face, 1, new) ;

	/* Find the distance from the optimal point to centroid */

        dnew[0] = new[0] - face->center[0] ;
        dnew[1] = new[1] - face->center[1] ;
        dnew[2] = new[2] - face->center[2] ;
        d = sqrt ( dnew[0] * dnew[0] +
                   dnew[1] * dnew[1] +
                   dnew[2] * dnew[2] ) ;
        dist = d / 1.0 ;

/*fprintf (stderr,"markos debug, newpt  = %f, %f, %f\n", new[0], new[1], new[2]) ;
fprintf (stderr,"markos debug, radius = %f\n", dist) ;*/

	/* Query the PT Tree */

	if (face->use == 0) factor =  1.5 ;
	else                factor =  3.0 ;
	imin[0] = new[0] - (factor*dist) ;
	imin[1] = new[1] - (factor*dist) ;
	imin[2] = new[2] - (factor*dist) ;
	imax[0] = new[0] + (factor*dist) ;
	imax[1] = new[1] + (factor*dist) ;
	imax[2] = new[2] + (factor*dist) ;

/* PJMesh - check if the search region of the face is inside the bounding box */
	if (pjmesh_use_box_test)
	{
		factor = 1.5;

		pjmesh_imin[0] = new[0] - (factor*dist*pjmesh_range_proportion) ;
		pjmesh_imin[1] = new[1] - (factor*dist*pjmesh_range_proportion) ;
		pjmesh_imin[2] = new[2] - (factor*dist*pjmesh_range_proportion) ;
		pjmesh_imax[0] = new[0] + (factor*dist*pjmesh_range_proportion) ;
		pjmesh_imax[1] = new[1] + (factor*dist*pjmesh_range_proportion) ;
		pjmesh_imax[2] = new[2] + (factor*dist*pjmesh_range_proportion) ;

		if (PMsh3DOutBox(pjmesh_imin, pjmesh_imax, pjmesh_box_min, pjmesh_box_max))
		{
			pjmesh_out_pt = 1;
		}

		if ((pjmesh_out_pt) && (face->use == 0))
		{
			//face->use = -1;
			face->use++;
			if (face->use == 3)
			{
				face->use = -1;
				face->layer = 0;
			}

			if ((face->use == 1) && (bdry_use1 == 0)) bdry_use1 = face;
			else if ((face->use == 2) && (bdry_use2 == 0)) bdry_use2 = face;
			else if ((face->use == -1) && (bdry_useNeg == 0)) bdry_useNeg = face;

/*//debug markos
			if (((face->verts[0] == 401) && (face->verts[1] == 433) && (face->verts[2] == 578)) ||
				((face->verts[0] == 433) && (face->verts[1] == 401) && (face->verts[2] == 465)) ||
				((face->verts[0] == 465) && (face->verts[1] == 401) && (face->verts[2] == 489)) ||
				((face->verts[0] == 433) && (face->verts[1] == 465) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 401) && (face->verts[2] == 578)) ||
				((face->verts[0] == 489) && (face->verts[1] == 578) && (face->verts[2] == 465)))
			{
				fprintf(stdout, "debug markos, setting face->use = %d for face %d, %d, %d\n", face->use, face->verts[0], face->verts[1], face->verts[2]);
			}
//end debug markos*/

			/*fprintf (stderr,"markos debug, face has search region out of bounding box\n");*/

			Msh3DBdryPush(face);

			continue;
		}
	}
/* end PJMesh - check if the search region of the face is inside the bounding box */

	point_result = QueryPointTree (imin, imax, &point_size) ;

	/* Debug PT_TREE query */

#if defined(PT_DEBU)
        if (point_size > num_nodes) {
         printf("Face has more nodes from PT_TREE search than all nodes\n") ;
	 printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
			                            face->verts[1],
						    face->verts[2]) ;
         printf("Total number of query nodes = %d\n", point_size) ;
         printf("Total number of front nodes = %d\n", num_nodes) ;
	 printf("\n") ;
        }
#endif

	/* Check the nodes from query */

        Msh3DHeapInit( point_size ) ;
        for ( m=0 ; m<point_size ; m++ )
        {
            id = point_result[m] ;
	    if (id >= num_nodes) continue ;
            if ( node_list[id].active_flag )
            {
             /* find the vector from the center to the new point
                and make sure that this dotted with the normal is
                positive */

             cand_vec[0] = node_list[id].coord[0] - face->center[0] ;
             cand_vec[1] = node_list[id].coord[1] - face->center[1] ;
             cand_vec[2] = node_list[id].coord[2] - face->center[2] ;

             dot = cand_vec[0]*face->r_x_s[0] +
                   cand_vec[1]*face->r_x_s[1] +
                   cand_vec[2]*face->r_x_s[2] ;

             if ( dot <= 0.0 ) continue ;

             /* verify if node is out of sphere centered in optimal node
                for the face and ratio equal of largest (smallest)
                distance from this node to any vertex of the face.
                This should be doen only the fase of ideal elements. */


             if (face->use == 0) {
               dist_vec[0] = node_list[id].coord[0] - new[0] ;
               dist_vec[1] = node_list[id].coord[1] - new[1] ;
               dist_vec[2] = node_list[id].coord[2] - new[2] ;
               did = sqrt ( dist_vec[0] * dist_vec[0] +
                            dist_vec[1] * dist_vec[1] +
                            dist_vec[2] * dist_vec[2] ) ;

               if ( did >= dist ) continue ;
             }


             /* the metric we are currently using is the square of
                the distance from the center of the face to the
                candidate node */

	     if (face->use == 0) {
              metric = Msh3DHeapSolidAngle( face, node_list,
                face->verts[0], face->verts[1], face->verts[2], id ) ;

//fprintf (stderr,"markos debug, setting metrics (%d) = %f, %f, %f, metric = %f\n", id, node_list[id].coord[0], node_list[id].coord[1], node_list[id].coord[2], metric);

              if( metric != 0.0 )    /* multiply by -1 because of heap */
               Msh3DHeapInsert( (-1.0*metric), id ) ;
             }
	     else {
	      metric = 0.0 ;
	      for ( j=0 ; j<3 ; j++ )
	       metric += cand_vec[j] * cand_vec[j] ;
	      metric = metric/dot ;

//fprintf (stderr,"markos debug, setting metrics (%d) = %f, %f, %f, metric = %f\n", id, node_list[id].coord[0], node_list[id].coord[1], node_list[id].coord[2], metric);

	      Msh3DHeapInsert( metric, id ) ;
             }
            }
        }
	QueryCleanPointTree ( ) ;
#else
	/* Check nodes that fall inside the sphere for the ideal
	 * phase or all nodes for topology phase */

        if (face->use == 0) Msh3DTreeNode (face, 1, new) ;

        Msh3DHeapInit( num_nodes ) ;
        for ( i=0 ; i<num_nodes ; i++ )
        {
            if ( node_list[i].active_flag )
            {
              /* find the vector from the center to the new point
                 and make sure that this dotted with the normal is
                 positive */

              cand_vec[0] = node_list[i].coord[0] - face->center[0] ;
              cand_vec[1] = node_list[i].coord[1] - face->center[1] ;
              cand_vec[2] = node_list[i].coord[2] - face->center[2] ;

              dot = cand_vec[0]*face->r_x_s[0] +
                    cand_vec[1]*face->r_x_s[1] +
                    cand_vec[2]*face->r_x_s[2] ;

              if ( dot <= 0.0 ) continue ;

              /* verify if node is out of sphere centered in optimal node
                 for the face and ratio equal of largest (smallest)
                 distance from this node to any vertex of the face.
                 This should be doen only the fase of ideal elements. */

              if (face->use == 0)
              {
               dist = 0 ;
               dnew[0] = new[0] - face->center[0] ;
               dnew[1] = new[1] - face->center[1] ;
               dnew[2] = new[2] - face->center[2] ;
               d = sqrt ( dnew[0] * dnew[0] +
                          dnew[1] * dnew[1] +
                          dnew[2] * dnew[2] ) ;
               dist = d / 1.0 ;

               dist_vec[0] = node_list[i].coord[0] - new[0] ;
               dist_vec[1] = node_list[i].coord[1] - new[1] ;
               dist_vec[2] = node_list[i].coord[2] - new[2] ;
               d = sqrt ( dist_vec[0] * dist_vec[0] +
                          dist_vec[1] * dist_vec[1] +
                          dist_vec[2] * dist_vec[2] ) ;

               if ( d >= dist ) continue ;
              }

              /* the metric we are currently using is the square of
                 the distance from the center of the face to the
                 candidate node */

              if (face->use == 0)
              {
               metric = Msh3DHeapSolidAngle( face, node_list,
                 face->verts[0], face->verts[1], face->verts[2], i ) ;

               if( metric != 0.0 )    /* multiply by -1 because of heap */
                Msh3DHeapInsert( (-1.0*metric), i ) ;
              }
              else
              {
               metric = 0.0 ;
               for ( j=0 ; j<3 ; j++ )
                 metric += cand_vec[j] * cand_vec[j] ;

               metric = metric/dot ;
               Msh3DHeapInsert( metric, i ) ;
              }
            }
        }
#endif

/* 2.4 start with the node with the best ranking.  Check to make
       sure it will form a valid element (i.e., does not intersect
       the current boundary).  If the element is invalid go on
       to the next.  If the element is valid, then look at the next
       candidate node.  If the two nodes have the same coordinates
       then use topology to determine the one we want. */

          DebugFail = 0 ;
          trials = 0 ;
          valid = 0 ;
	  alloc = 1 ;

          while ( !valid ) {

            /* extract a node from the heap based in its metric */

            node_indx = Msh3DHeapExtract( &metric ) ;
            if ((node_indx == face->verts[0]) ||
                (node_indx == face->verts[1]) ||
                (node_indx == face->verts[2])) continue ;

			/* here a node was found to make the element (node_indx was
               extracted from the heap). Check its validity */

            if ( node_indx != -1 ) {

             /* node avaiable in the heap */

             check = 1 ;

             /* check validity for node choosen */

             if (check)
              check = Msh3DCheckVolum( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckShape( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckValid( face, node_list, node_indx, node_bdry,
                        -1, 0, *generated_elements, num_gen_elements ) ;
             if (check)
              check = Msh3DCheckTrans( face, node_list, node_indx, node_bdry,
                        -1, 0, *generated_elements, num_gen_elements ) ;

/* PJMesh - for the topology phase, a tetrahedron may be created if all 4 faces
			existed before, even if the search region crosses the bounding box */
			if (pjmesh_use_box_test)
			{
			 if (check)
			  check = PMsh3DCheckExist( face, node_list, node_indx, pjmesh_out_pt );
			}
/* end PJMesh - for the topology phase, a tetrahedron may be created if all 4 faces
			existed before, even if the search region crosses the bounding box */

             /* set validity for node choosen */

             valid = check ;
             if ( valid ) {
//fprintf (stderr,"markos debug, candidate = %f, %f, %f, metric = %f\n", node_list[node_indx].coord[0], node_list[node_indx].coord[1], node_list[node_indx].coord[2], -metric);

                 next_indx = Msh3DHeapExtract( &next_metric ) ;
                 if ( next_indx == -1 ) break ;
                 if (( next_metric == metric ) &&
                     ( node_list[node_indx].coord[0] ==
                       node_list[next_indx].coord[0] ) &&
                     ( node_list[node_indx].coord[1] ==
                       node_list[next_indx].coord[1] ) &&
                     ( node_list[node_indx].coord[2] ==
                       node_list[next_indx].coord[2] )) {
                    /* fprintf( stderr, "Determine The node we want \n" ) ; */

//fprintf (stderr,"markos debug, equal metric candidate = %f, %f, %f, metric = %f\n", node_list[next_indx].coord[0], node_list[next_indx].coord[1], node_list[next_indx].coord[2], -next_metric);

                    node_indx = Msh3DDetNode( face, node_list,
                                              node_indx, next_indx ) ;
                 }
#if PT_DEBU
		 /* check if the node chosen is inside the PT tree and
		  * also print the comparison between nodes that would
		  * be checked with and without the use of the PT_TREE */

                 Msh3DTreeNode (face, 1, new) ;
                 dnew[0] = new[0] - face->center[0] ;
                 dnew[1] = new[1] - face->center[1] ;
                 dnew[2] = new[2] - face->center[2] ;
                 d = sqrt ( dnew[0] * dnew[0] +
                            dnew[1] * dnew[1] +
                            dnew[2] * dnew[2] ) ;
                 dist = d / 1.0 ;
	         if (face->use == 0) factor =  1.5 ;
	         else                factor =  3.0 ;
	         imin[0] = new[0] - (factor*dist) ;
	         imin[1] = new[1] - (factor*dist) ;
	         imin[2] = new[2] - (factor*dist) ;
	         imax[0] = new[0] + (factor*dist) ;
	         imax[1] = new[1] + (factor*dist) ;
	         imax[2] = new[2] + (factor*dist) ;
	         point_result = QueryPointTree (imin, imax, &point_size) ;
		 found = 0 ;
		 print = 0 ;
                 for ( m=0 ; m<point_size ; m++ )
                 {
                  id = point_result[m] ;
	          if (id >= num_nodes) continue ;
		  if (id == node_indx) {
		   found = 1 ;
		   break ;
                  }
                 }
		 if (!found) {
		  fprintf (stderr,"Node=%d for Face=%d %d %d not in PT_TREE\n",
			   node_indx,      face->verts[0],
			   face->verts[1], face->verts[2]) ;
                 }
		 if (print) {
		  curol = 0 ;
	          for (m=0; m < num_nodes; m++) {
                   if (node_list[m].active_flag) curol++ ;
                  }
	          curfa = point_size ;
                  printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
		 		                             face->verts[1],
							     face->verts[2]) ;
	          if (face->use == 0)
                   printf("Geometry: Normal=%d Pt_Tree=%d\n",curol,curfa) ;
	          if (face->use == 1)
                   printf("Topology: Normal=%d Pt_Tree=%d\n",curol,curfa) ;
	          if (face->use == 2)
                   printf("Tracking: Normal=%d Pt_Tree=%d\n",curol,curfa) ;
		  printf ("\n") ;
		 }
	         QueryCleanPointTree ( ) ;
#endif
             }
             else {
                trials += 1 ;
             }
            }

            /* here no node was found to make the element (no node_indx was
	       found). Try insertion of new node by an baricentric procedure
               that represents the optimal node for this face */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = 1 ;
             alloc     = 1 ;

             /* try insertion of optimal node if it's in the phase of
                ideal elements */

             if (face->use == 0)
             {
               /* try insertion of new node */

               for (k=1; k<=1; k++)
	       {
                /* node inserted */

                check = Msh3DInsertNewNodesBar (face, &num_nodes, k,
                          &alloc, &face_pop) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckOptim(face, num_nodes-1, num_org_nodes,
                  num_org_faces, original_nodes, original_faces) ;
                if (check)
                 check = Msh3DCheckValid( face, node_list, num_nodes-1,
                  -1, -1, 0, *generated_elements, num_gen_elements ) ;

		/* check if new node is inside the octree box. if it is
		 * not it means that we're trying to mesh the outwards
		 * and because of this it will never converge. */

		if (check) {
		 check = Msh3DCheckInbox( face, node_list, num_nodes-1 ) ;
		 if (!check) {
                   Msh3DHeapDelete() ;
                   Msh3DPolyFaceFreeAll() ;
                   //Msh3DFaceFreeAll() ;
                   PMsh3DFaceFreeAll() ;
                   Msh3DTestFreeAll() ;
                   Msh3DAdjFreeAll() ;
                   Msh3DAdjIniFreeAll() ;
                   Msh3DAdjElemFreeAll() ;
                   Msh3DAdjNodeFreeAll() ;
                   Msh3DRangeFreeAll() ;
                   Msh3DPointFreeAll() ;
		   return (0) ;
                 }
		}

                /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = num_nodes - 1 ;
               }
               else {
                num_nodes-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }

            /* here no node was found to make the element (no node_indx was
	       found). Try insertion of new node by an adjacent procedure if
	       this face is the first face that went one time to the end of
	       the stack (bdry_try face represented by use equal to 1) */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = -1 ;
             alloc     =  1 ;

             /* try insertion of optimal node if it's in the phase of
                insert elements */

/* PJmesh - the insertion of new nodes cannot happen in the topology phase if
			the search region crosses the bounding box */
             //if (face->use != 0 /*face->use == 1*/)
             if ((face->use != 0 /*face->use == 1*/) &&
				 ((!pjmesh_use_box_test) || (!pjmesh_out_pt)))
/* end PJmesh - the insertion of new nodes cannot happen in the topology phase if
			the search region crosses the bounding box */
             {
               /* try insertion of new node */

               for (k=1; k<=NEW_NODES; k++)
	       {
                /* node inserted */

                check = Msh3DInsertNewNodesAdj (face, &num_nodes, k,
                          &alloc, &face_pop, 1) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckValid( face, node_list, num_nodes-1,
                  -1, k, 1, *generated_elements, num_gen_elements ) ;

		/* check if new node is inside the octree box. if it is
		 * not it means that we're trying to mesh the outwards
		 * and because of this it will never converge. */

		if (check) {
		 check = Msh3DCheckInbox( face, node_list, num_nodes-1 ) ;
		 if (!check) {
                   Msh3DHeapDelete() ;
                   Msh3DPolyFaceFreeAll() ;
                   //Msh3DFaceFreeAll() ;
                   PMsh3DFaceFreeAll() ;
                   Msh3DTestFreeAll() ;
                   Msh3DAdjFreeAll() ;
                   Msh3DAdjIniFreeAll() ;
                   Msh3DAdjElemFreeAll() ;
                   Msh3DAdjNodeFreeAll() ;
                   Msh3DRangeFreeAll() ;
                   Msh3DPointFreeAll() ;
		   return (0) ;
                 }
		 /*if (!check) return (2) ;*/
		}

                /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = num_nodes - 1 ;
               }
               else {
                num_nodes-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }
          }

	  /* if no node was found even with the insertion of new nodes by
	     procedures described above, return to main driver and write
             error code if this face is the first face that went two times
             to to the end of stack (bdry_try face represented by use equal
             to 2) or push this face into the end of stack and try next
             face. Else if a node was found, reset the first face that went
             to the end of stack (bdry_try face) and its use, considering
             that if this face has use equal to 0 then reset bdry_try face
             to no one face and continue trying as before, else if its use
             is equal to 1 then reset bdry_try face to current one and
             continue trying now inserting a new node always when needed */

	  if ( node_indx == -1 ) {
           if (face->use == 2) {
//#if (!ENABLE_BACK_TRACKING) //Markos
			if ((!pjmesh_force_back_track) && (pjmesh_use_box_test))
			{
             face->use = -1;
             if (face->use == -1) face->layer = 0;

             if ((face->use == 1) && (bdry_use1 == 0)) bdry_use1 = face;
			 else if ((face->use == 2) && (bdry_use2 == 0)) bdry_use2 = face;
			 else if ((face->use == -1) && (bdry_useNeg == 0)) bdry_useNeg = face;
              Msh3DBdryPush (face) ;
             break;
			}
//#endif //#if (!ENABLE_BACK_TRACKING) //Markos
             Msh3DBdryPush (face) ;
             if ( Msh3DCorrePoly (&num_nodes, num_gen_elements,
                                 generated_elements, 1, 1, 0, 1, pjmesh_use_box_test) ) break ;
             Msh3D_errno = Msh3D_INSERT_NO_VALID_NODES ;
             Msh3DHeapDelete() ;
             Msh3DPolyFaceFreeAll() ;
             //Msh3DFaceFreeAll() ;
             PMsh3DFaceFreeAll() ;
	     Msh3DTestFreeAll() ;
             Msh3DAdjFreeAll() ;
             Msh3DAdjIniFreeAll() ;
             Msh3DAdjElemFreeAll() ;
             Msh3DAdjNodeFreeAll() ;
#if VER_BOTH
	     Msh3DNodeFreeAll() ;
	     Msh3DRangeFreeAll() ;
	     Msh3DPointFreeAll() ;
	     return(0);
#else
             strcpy( Msh3D_err_msg,
              "Msh3DBdryContraction : Insertion of new nodes wasn't enough" ) ;
             return(0);
#endif
           }
           else {
/* PJmesh - in topology phase, the use = -1 was set before */
            if (pjmesh_use_box_test)
			{
				if (face->use != -1)
					face->use++ ;

				if (face->use == 3)
				{
					face->use = -1;
				}
			}
			else
			{
				face->use++;
			}
/* end PJmesh - in topology phase, the use = -1 was set before */
             face_pop++ ;
#if OLD_BTRY
	     bdry_use1 = 0 ;
	     bdry_use2 = 0 ;
	     bdry_useNeg = 0 ;
#else
	     if ((face->use==1) && (bdry_use1==0))
  	      bdry_use1 = face ;
	     if ((face->use==2) && (bdry_use2==0))
	      bdry_use2 = face ;
		 if ((face->use==-1) && (bdry_useNeg==0))
	      bdry_useNeg = face ;
#endif
             Msh3DBdryPush (face) ;
             continue ;
           }
          }
	  else {
           if (face->use == 0) {
            face_draw++ ;
            face_done++ ;
           }
          }

	  startid++ ;

#if defined(AD_TREE) || defined(AD_DEBU)
	  /* Rebuild the PT tree to balance it better. We should do
	   * it in two situations: a) every 4500 generated elements
	   * unconditionally or b) every 100 generated elements, for
	   * the topology-phase, if the number of active faces is
	   * smaller than the number of faces stored in the AD tree.
	   * The reason to do b) only in the topology-phase is
	   * because in the geometry-phase the range for search in
	   * the tree is very small, and so it is very unlikely
	   * that the number of active faces will be higher.  */
	  if ((startid % 4500) == 0) {
	   lnum = 0 ;
	   fur = 0 ;
           fprintf( stderr,"\t\t Rebuilding range tree:\n") ;
           CleanRangeTree( ) ;
	   Msh3DBdryReset() ;
	   while (bface = Msh3DBdryNext ()) lnum++ ;
	   //added by markos
       lnum++; //to account for the current face that will be advanced
       //end added by markos
	   min = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
	   max = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
	   fid = (int *)Msh3DCalloc(lnum, sizeof(int)) ;
	   Msh3DBdryReset() ;
	   while (bface = Msh3DBdryNext ()) {
	    min[fur][0] = bface->min[0] ;
	    min[fur][1] = bface->min[1] ;
	    min[fur][2] = bface->min[2] ;
	    max[fur][0] = bface->max[0] ;
	    max[fur][1] = bface->max[1] ;
	    max[fur][2] = bface->max[2] ;
	    fid[fur] = bface->adtreeindx ;
	    fur++ ;
           }
       //added by markos
       {
        bface = face;
        min[fur][0] = bface->min[0] ;
        min[fur][1] = bface->min[1] ;
        min[fur][2] = bface->min[2] ;
        max[fur][0] = bface->max[0] ;
        max[fur][1] = bface->max[1] ;
        max[fur][2] = bface->max[2] ;
        fid[fur] = bface->adtreeindx ;
        fur++ ;
       }
       //end added by markos
#if DSP_TIME
           init_tree_time = clock( );
#endif
           InitRangeTree (fur,min,max,fid) ;
#if DSP_TIME
           tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
           fprintf( stderr,"\t\t Size of range tree: %8d fac.\n", fur) ;
           fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
           fprintf( stderr,"\n") ;
#endif
	   Msh3DFree (min) ;
	   Msh3DFree (max) ;
	   Msh3DFree (fid) ;
          }
	  else if ((startid % 100) == 0) {
	   if (face->use > 0) {
	     lnum = 0 ;
	     fur = 0 ;
             minc[0] = face->min[0] ;
             minc[1] = face->min[1] ;
             minc[2] = face->min[2] ;
             maxc[0] = face->max[0] ;
             maxc[1] = face->max[1] ;
             maxc[2] = face->max[2] ;
             for (n=1 ; n<4 ; n++ ) {
              node_id[0] = face->verts[n-1] ;
              node_id[1] = face->verts[n%3] ;
              node_id[2] = node_indx ;
              for (l=0; l < 3; l++) {
               maxf[l] = node_list[node_id[0]].coord[l] ;
               minf[l] = node_list[node_id[0]].coord[l] ;
               if (maxf[l] < node_list[node_id[1]].coord[l])
                maxf[l] = node_list[node_id[1]].coord[l] ;
               if (minf[l] > node_list[node_id[1]].coord[l])
                minf[l] = node_list[node_id[1]].coord[l] ;
               if (maxf[l] < node_list[node_id[2]].coord[l])
                maxf[l] = node_list[node_id[2]].coord[l] ;
               if (minf[l] > node_list[node_id[2]].coord[l])
                minf[l] = node_list[node_id[2]].coord[l] ;
              }
              if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
              if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
              if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
              if (minc[0] > minf[0]) minc[0] = minf[0] ;
              if (minc[1] > minf[1]) minc[1] = minf[1] ;
              if (minc[2] > minf[2]) minc[2] = minf[2] ;
             }
	     range_result = QueryRangeTree(minc,maxc,&range_size) ;
	     curfa = range_size ;
	     QueryCleanRangeTree ( ) ;
	     Msh3DBdryReset() ;
	     while (bface = Msh3DBdryNext ()) lnum++ ;
	     if (curfa > lnum) {
              fprintf( stderr,"\t\t Rebuilding range tree:\n") ;
              CleanRangeTree( ) ;
          //added by markos
		  lnum++; //to account for the current face that will be advanced
		  //end added by markos
	      min = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
	      max = (RangeCoord *)Msh3DCalloc(lnum, sizeof(RangeCoord)) ;
	      fid = (int *)Msh3DCalloc(lnum, sizeof(int)) ;
	      Msh3DBdryReset() ;
	      while (bface = Msh3DBdryNext ()) {
	       min[fur][0] = bface->min[0] ;
	       min[fur][1] = bface->min[1] ;
	       min[fur][2] = bface->min[2] ;
	       max[fur][0] = bface->max[0] ;
	       max[fur][1] = bface->max[1] ;
	       max[fur][2] = bface->max[2] ;
	       fid[fur] = bface->adtreeindx ;
	       fur++ ;
              }
          //added by markos
		  {
		   bface = face;
		   min[fur][0] = bface->min[0] ;
		   min[fur][1] = bface->min[1] ;
		   min[fur][2] = bface->min[2] ;
		   max[fur][0] = bface->max[0] ;
		   max[fur][1] = bface->max[1] ;
		   max[fur][2] = bface->max[2] ;
		   fid[fur] = bface->adtreeindx ;
		   fur++ ;
		  }
		  //end added by markos
#if DSP_TIME
              init_tree_time = clock( );
#endif
              InitRangeTree (fur,min,max,fid) ;
#if DSP_TIME
              tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
              fprintf( stderr,"\t\t Size of range tree: %8d fac.\n", fur) ;
              fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
              fprintf( stderr,"\n") ;
#endif
	      Msh3DFree (min) ;
	      Msh3DFree (max) ;
	      Msh3DFree (fid) ;
	     }
           }
	  }
#endif

#if defined(PT_TREE) || defined(PT_DEBU)
	  /* Rebuild the PT tree to balance it better. We should do
	   * it in two situations: a) every 4500 generated elements
	   * unconditionally or b) every 100 generated elements, for
	   * the topology-phase, if the number of active nodes is
	   * smaller than the number of nodes stored in the PT tree.
	   * The reason to do b) only in the topology-phase is
	   * because in the geometry-phase the range for search in
	   * the tree is very small, and so it is very unlikely
	   * that the number of active nodes will be higher.  */
	  if ((startid % 4500) == 0) {
           fprintf( stderr,"\t\t Rebuilding point tree:\n") ;
           CleanPointTree( ) ;
           ids = (int *)Msh3DCalloc(num_nodes,sizeof(int));
           cds = (PointCoord *)Msh3DCalloc(num_nodes,sizeof(PointCoord));
	   dur = 0 ;
           for ( i=0 ; i<num_nodes ; i++ )
           {
            if ( node_list[i].active_flag ) {
             ids[dur] = i ;
	     cds[dur][0] = node_list[i].coord[0] ;
	     cds[dur][1] = node_list[i].coord[1] ;
	     cds[dur][2] = node_list[i].coord[2] ;
	     ++dur ;
            }
	   }
#if DSP_TIME
           init_tree_time = clock( );
#endif
           InitPointTree (dur,cds,ids) ;
#if DSP_TIME
           tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
           fprintf( stderr,"\t\t Size of point tree: %8d nod.\n", dur) ;
           fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
           fprintf( stderr,"\n") ;
#endif
	   Msh3DFree (cds) ;
	   Msh3DFree (ids) ;
          }
	  else if ((startid % 100) == 0) {
           if (face->use > 0) {
            Msh3DTreeNode (face, 1, new) ;
            dnew[0] = new[0] - face->center[0] ;
            dnew[1] = new[1] - face->center[1] ;
            dnew[2] = new[2] - face->center[2] ;
            d = sqrt ( dnew[0] * dnew[0] +
                       dnew[1] * dnew[1] +
                       dnew[2] * dnew[2] ) ;
            dist = d / 1.0 ;
	    factor =  3.0 ;
	    imin[0] = new[0] - (factor*dist) ;
	    imin[1] = new[1] - (factor*dist) ;
	    imin[2] = new[2] - (factor*dist) ;
	    imax[0] = new[0] + (factor*dist) ;
	    imax[1] = new[1] + (factor*dist) ;
	    imax[2] = new[2] + (factor*dist) ;
	    point_result = QueryPointTree (imin, imax, &point_size) ;
            curol = 0 ;
	    for (m=0; m < num_nodes; m++) {
             if (node_list[m].active_flag) curol++ ;
            }
	    curfa = point_size ;
	    QueryCleanPointTree ( ) ;
	    if (curfa > curol ) {
             fprintf( stderr,"\t\t Rebuilding point tree:\n") ;
             CleanPointTree( ) ;
             ids = (int *)Msh3DCalloc(num_nodes,sizeof(int));
             cds = (PointCoord *)Msh3DCalloc(num_nodes,sizeof(PointCoord));
	     dur = 0 ;
             for ( i=0 ; i<num_nodes ; i++ )
             {
              if ( node_list[i].active_flag ) {
               ids[dur] = i ;
	       cds[dur][0] = node_list[i].coord[0] ;
	       cds[dur][1] = node_list[i].coord[1] ;
	       cds[dur][2] = node_list[i].coord[2] ;
	       ++dur ;
              }
	     }
#if DSP_TIME
             init_tree_time = clock( );
#endif
             InitPointTree (dur,cds,ids) ;
#if DSP_TIME
             tot_tree_time = (clock( ) - init_tree_time)/CLOCKS_PER_SEC;
             fprintf( stderr,"\t\t Size of point tree: %8d nod.\n", dur) ;
             fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_tree_time);
             fprintf( stderr,"\n") ;
#endif
	     Msh3DFree (cds) ;
	     Msh3DFree (ids) ;
            }
	   }
	  }
#endif

/* 2.5 add the new element to the element list, update the
       boundary face list, and update the active node list. */

//fprintf (stderr,"markos debug, adv to pt = %d, %f, %f, %f\n", node_indx, node_list[node_indx].coord[0], node_list[node_indx].coord[1], node_list[node_indx].coord[2]);
//fprintf (stderr,"markos debug, solid angle = %f\n", Msh3DHeapSolidAngle( face, node_list, face->verts[0], face->verts[1], face->verts[2], node_indx ) );

	  /* Add new element to the element list */
          Msh3DAddElem( num_gen_elements, generated_elements,
                        face, node_indx ) ;

	  /* Update the front */
          Msh3DAddFaces( face, node_list, node_indx, num_gen_elements ) ;

          if ( *num_gen_elements == gbl_quit ) {
              Msh3DBdryDump( node_list ) ;
            return(0) ;
          }
    }

    //added by markos
    if (pjmesh_use_box_test)
    {
		/* PJmesh - all remaining faces should not be changed in improvement back-tracking */

		Msh3DBdryReset();

		while (face = Msh3DBdryNext())
		{
			if (face->use != -1)
			{
				face->use = -1;
			}
		}
    }
    //end added by markos

/* 2.6 smooth the mesh considering all elements done. Also correct bad
       shape elements when necessary. */

    fprintf (stderr,"Quality Improvement:") ;
    fprintf( stderr,"\n") ;

#if DSP_TIME
    fprintf( stderr, "\n" );
    fprintf( stderr, "	Generating improved elems.............\n" );
    fprintf( stderr, "\n" );
#endif

   do {

    /* get number of boundary nodes. In the first step, that is after the
       creation of the mesh, the number is the original one; after that,
       only smooth the nodes created to fix bad shape elements. */

    if (step == 0) {
     bdr_nodes = nbdrynode ;
     cur_nodes = num_nodes ;
    }
    else {
     bdr_nodes = cur_nodes ;
     cur_nodes = num_nodes ;
    }

    /* smooth the internal nodes, because the boundary nodes can't be changed */

//#if ENABLE_SMOOTHING //Markos
    Msh3DSmooth( num_nodes, bdr_nodes, *num_gen_elements, *generated_elements, pjmesh_use_box_test ) ;
//#endif //#if ENABLE_SMOOTHING //Markos

    /* correct the shape of sliver elements */

//PMsh3DPrintBdryList("\n\n\nwill now work on back-tracking to improve (before Msh3DShapePoly)\n", NULL, 0, 1, 1);

//if (PMshCountPartition == PMshStudiedPartition) PMsh3DPrintBdryList("\nwill now work to advance\n", face, 1, 1, 0);

//#if ENABLE_IMPROVING_BACK_TRACKING //Markos
if ((pjmesh_force_back_track) || (!pjmesh_use_box_test))
{
#if DSP_DELE
    Msh3DShapePoly (&num_nodes, num_gen_elements, generated_elements,
                    &shape, &step, 0, 1, pjmesh_use_box_test) ;
#else
    Msh3DShapePoly (&num_nodes, num_gen_elements, generated_elements,
                    &shape, &step, 0, 0, pjmesh_use_box_test) ;
#endif
}
//#endif //#if ENABLE_IMPROVING_BACK_TRACKING //Markos

//PMsh3DPrintBdryList("\n\n\nwill now work on back-tracking to improve (after Msh3DShapePoly)\n", NULL, 0, 1, 1);

    /* stop if all elements have a good shape or after some steps */

   } while( shape ) ;

//if (PMshCountPartition == PMshStudiedPartition) PMsh3DPrintBdryList("\nwill now work to advance\n", face, 1, 1, 0);

/* 2.7 correct elements list because some elements in the list could
       have been deleted during the Msh3DCorrePoly and Msh3DShapePoly
       steps */

#if DSP_TIME
    init_time = clock( );
    fprintf( stderr, "	Deleting bad-shaped elems............\n" );
    fprintf( stderr, "\n" );
#endif

   aux_num_elems = (*num_gen_elements) * 4 ;
   aux_num_nodes = (nbdrynode+nintnode) ;
   for( i = 0; i < aux_num_nodes;  i++ ) {
    node_list[i].active_flag = 0 ;
   }
   aux_vec_elems = (int *)Msh3DMalloc( aux_num_elems * sizeof(int) ) ;
   aux_vec_nodes = (int *)Msh3DMalloc( aux_num_nodes * sizeof(int) ) ;
   aux_num_elems = 0 ;
   aux_num_nodes = 0 ;
   for( i = 0; i < (*num_gen_elements); i++ ) {
    ind = i*4 ;
    if( ((*generated_elements)[ind+0] == -1) &&
        ((*generated_elements)[ind+1] == -1) &&
        ((*generated_elements)[ind+2] == -1) &&
        ((*generated_elements)[ind+3] == -1)  )
    {
     continue ;
    }
    else
    {
     nind = aux_num_elems*4 ;
     for( j = 0; j < 4; j++ ) {
      aux_vec_elems[nind+j] = (*generated_elements)[ind+j] ;
      node_list[(*generated_elements)[ind+j]].active_flag = 1 ;
     }
     aux_num_elems++ ;
    }
   }
   for( i = 0; i < (nbdrynode+nintnode); i++ ) {
    if(( node_list[i].active_flag != 1 ) && (i >= nbdrynode))
    {
     continue ;
    }
    else {
     aux_vec_nodes[i] = aux_num_nodes ;
     aux_num_nodes++ ;
     node_list[aux_vec_nodes[i]].active_flag = 1;
     for( j = 0; j < 3; j++ ) {
      node_list[aux_vec_nodes[i]].coord[j] = node_list[i].coord[j] ;
     }
    }
   }
   num_nodes         = aux_num_nodes ;
   *num_gen_elements = aux_num_elems ;
   for( i = 0; i < aux_num_elems; i++ ) {
    ind = i*4 ;
    for( j = 0; j < 4; j++ ) {
     (*generated_elements)[ind+j] = aux_vec_nodes[aux_vec_elems[ind+j]] ;
    }
   }

   //added by markos
	if (pjmesh_use_box_test)
	{
		face = bdry_stack;

		while (face)
		{
			for (i = 0; i < 3; i++)
			{
				face->verts[i] = aux_vec_nodes[face->verts[i]];
			}

			face = face->next;
		}
	}
   //end added by markos

   Msh3DFree( aux_vec_nodes ) ;
   Msh3DFree( aux_vec_elems ) ;

#if DSP_TIME
    tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
    fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *num_gen_elements);
    fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
    fprintf( stderr,"\n") ;
#endif

/* 2.8 include all new generated nodes in the vector of internal_nodes */

   all_nodes = (*num_int_nodes) ;
   if( !all_nodes ) {
    all_nodes = (num_nodes - (num_org_nodes + (*num_int_nodes))) ;
    (*internal_nodes) = (double *)Msh3DMalloc(
            all_nodes * 3 * sizeof(double) ) ;
	//added by markos
	(*tinternal_nodes) = (int *)Msh3DCalloc(all_nodes, sizeof(int) ) ;
	//end added by markos
   }
   else {
    all_nodes += (num_nodes - (num_org_nodes + (*num_int_nodes))) ;
    (*internal_nodes) = (double *)Msh3DRealloc( (*internal_nodes),
            all_nodes * 3 * sizeof(double) ) ;
	//added by markos
	(*tinternal_nodes) = (int *)Msh3DRealloc( (*tinternal_nodes),
			all_nodes * sizeof(int) ) ;
	//end added by markos
   }

	//added by markos
	if (pjmesh_num_original_elements != 0)
	{
		(*num_int_nodes) = 0;
	}
	//end added by markos

   for ( i=(num_org_nodes+(*num_int_nodes)); i < num_nodes; i++ ) {
    if( node_list[i].active_flag == 1 ) {
     for( j = 0; j < 3; j++)
      (*internal_nodes)[(*num_int_nodes)*3+j] = node_list[i].coord[j] ;
     (*num_int_nodes)++ ;
    }
   }

/* 2.9 draw the mesh */

#if DSP_MESH
    if ( FDEBUG ) {
      GdbClear () ;
      GdbBuildMesh(offset, *generated_elements,num_gen_elements) ;
      GdbIni ("Mesh3d Generator - Generated Mesh");
      GdbDrawMesh (offset, *generated_elements,num_gen_elements) ;
    }
#endif

/* 2.10 using the look-up table for nodes and faces, update the mesh */

   //commented by markos
   //if ((*num_int_nodes) != 0)
   // (*tinternal_nodes) = (int *)Msh3DCalloc((*num_int_nodes), sizeof(int) ) ;
   //end commented by markos
   for( i = 0; i < (*num_gen_elements); i++ ) {
    for( j = 0; j < 4; j++ ) {
     if( (*generated_elements)[i*4+j] < num_org_nodes) {
      nodeid = (*generated_elements)[i*4+j] ;
      (*generated_elements)[i*4+j] = toriginal_nodes[nodeid] ;
     }
     else {
      nodeid = (*generated_elements)[i*4+j] ;
      node_list[nodeid].real_id  = nodeid + offset ;
      (*tinternal_nodes)[nodeid-num_org_nodes] = nodeid + offset ;
      (*generated_elements)[i*4+j] = (*tinternal_nodes)[nodeid-num_org_nodes] ;
     }
    }
   }

/* 2.11 release memory and return status indicating that a mesh was generated */

    Msh3DHeapDelete() ;
    Msh3DPolyFaceFreeAll() ;
    //Msh3DFaceFreeAll() ;
    //PMsh3DFaceFreeAll() ;
    Msh3DTestFreeAll() ;
    Msh3DAdjFreeAll() ;
    Msh3DAdjIniFreeAll() ;
    Msh3DAdjElemFreeAll() ;
    Msh3DAdjNodeFreeAll() ;
    /* PJmesh - num_org_faces might be 0 */
    if (num_org_faces > 0)
		Msh3DRangeFreeAll() ;
    Msh3DPointFreeAll() ;

    return (1) ;
}

/* -------------------------------------------------------------------
** Msh3DSmooth - driver for nodal smoothing.
*/

static int Msh3DSmooth(
    int           num_nodes,
    int           bdr_nodes,
    int           num_elems,
    int          *elements,
    int           pjmesh_use_box_test )
{
    Msh3DBdryNodeList smoo_list ;
    Msh3DBdryNode     node ;
    Msh3DAdjElem      adj_elem ;
    Msh3DAdjElemList  elems ;
    Msh3DBdryFaceRec  face ;
    int               i, j, k, n, id, node_indx, check ;
    double            x, y, z, r, w, wj ;

/* 3.1 Allocate memory for smooth nodes vector */

    smoo_list = (Msh3DBdryNodeList)Msh3DMalloc( num_nodes *
                              sizeof(Msh3DBdryNodeRec) ) ;

/* 3.2 Initiate list of all elements adjacent to a given node and
       initiate list of smoothed nodes vector */

    for( i = 0; i < num_nodes; i++ ) {
     node_list[i].elems = (Msh3DAdjElemList)0 ;
     smoo_list[i].coord[0] = node_list[i].coord[0] ;
     smoo_list[i].coord[1] = node_list[i].coord[1] ;
     smoo_list[i].coord[2] = node_list[i].coord[2] ;
    }

/* 3.3 Build a list of all elements adjacent to a given node */

#if OLD_SMOO
    /* Do it for all nodes on the mesh */
    for( i = 0; i < num_elems; i++ ) {
     if( elements[i*4] == -1 ) continue ;
     for( j = 0; j < 4; j++ ) {
      node = &node_list[elements[(i*4)+j]] ;
      adj_elem = Msh3DAdjElemAlloc( ) ;
      if (!adj_elem) {
       Msh3DAdjElemFreeAll( ) ;
       return 0 ;
      }
      adj_elem->next = node->elems ;
      adj_elem->elem = i ;
      node->elems = adj_elem ;
     }
    }
#else
    /* Do it only for the nodes to be smoothed */
    for( i = 0; i < num_elems; i++ ) {
     if( elements[i*4] == -1 ) continue ;
     for( j = 0; j < 4; j++ ) {
      if (elements[(i*4)+j] >= bdr_nodes) {
       node = &node_list[elements[(i*4)+j]] ;
       adj_elem = Msh3DAdjElemAlloc( ) ;
       if (!adj_elem) {
        Msh3DAdjElemFreeAll( ) ;
        return 0 ;
       }
       adj_elem->next = node->elems ;
       adj_elem->elem = i ;
       node->elems = adj_elem ;
      }
     }
    }
#endif

/* 3.4 Move each internal node to the centroid of all its adjacent
       nodes defined by its adjacent elements. Each internal node
       only will be moved if doesn't affect the consistency of its
       adjacent elements, so consistency tests are necessary after
       each move. */

    for( i = 0; i < Msh3D_SMOOTH_STEP; i++ ) {
     for( j = 0; j < num_nodes; j++ ) {
      /* Smooth only internal nodes because boundary ones can't be moved */
      if( j >= bdr_nodes ) {

/* PJMesh - nodes in output front cannot be moved */
		if (pjmesh_use_box_test)
		{
			/*int pjmesh_outter_face = 0;

			Msh3DAdjFaceList face = node_list[j].faces;

			while (face)
			{
				if (PMsh3DBoundaryFace(face->face))
				{
					pjmesh_outter_face = 1;

					break;
				}

				face = face->next;
			}

			if (pjmesh_outter_face)
			{
				continue;
			}*/

			/* if there is a face adjacent to this node, then this node belongs
			   to the updated front, and cannot be moved */

			if (node_list[j].faces)
			{
				continue;
			}
		}

/* end PJMesh - nodes in output front cannot be moved */

       /* Compute coordinates (x,y,z) and weight (w) for node */
       n = 0 ;
       x = y = z = w = 0.0 ;
       elems = node_list[j].elems ;
       while( elems ) {
        for( k = 0; k < 4; k++ ) {
         id = elements[(elems->elem*4)+k] ;
         if( id != j ) {
          wj = 1.0 ;
          x += (wj * (node_list[id].coord[0] - node_list[j].coord[0])) ;
          y += (wj * (node_list[id].coord[1] - node_list[j].coord[1])) ;
          z += (wj * (node_list[id].coord[2] - node_list[j].coord[2])) ;
          w += wj ;
          n++ ;
         }
        }
        elems = elems->next ;
       }
       /* If weight is above 0 then smooth node */
       elems = node_list[j].elems ;
       if( elems )
       {
        if( w > 0.0 ) {
         smoo_list[j].coord[0] = node_list[j].coord[0] + (REL_FACTOR * (x / w)) ;
         smoo_list[j].coord[1] = node_list[j].coord[1] + (REL_FACTOR * (y / w)) ;
         smoo_list[j].coord[2] = node_list[j].coord[2] + (REL_FACTOR * (z / w)) ;
        }
       }

//fprintf (stderr,"markos debug, smoothing node = %d, old_coord = (%f, %f, %f), new_coord = (%f, %f, %f)\n", j, node_list[j].coord[0], node_list[j].coord[1], node_list[j].coord[2], smoo_list[j].coord[0], smoo_list[j].coord[1], smoo_list[j].coord[2]) ;

       /* Check consistency for the smoothed node */
       elems = node_list[j].elems ;
       while( elems ) {
        face.verts[0] = elements[(elems->elem*4)+0] ;
        face.verts[1] = elements[(elems->elem*4)+1] ;
        face.verts[2] = elements[(elems->elem*4)+2] ;
        node_indx     = elements[(elems->elem*4)+3] ;
        check = Msh3DCheckConsi( &face, smoo_list, node_indx ) ;
        if( !check ) break ;
        elems = elems->next ;
       }
       /* If smoothing is not valid, recover original values for node */
       if( elems ) {

//fprintf (stderr,"markos debug, smoothing invalid\n") ;

        smoo_list[j].coord[0] = node_list[j].coord[0] ;
        smoo_list[j].coord[1] = node_list[j].coord[1] ;
        smoo_list[j].coord[2] = node_list[j].coord[2] ;
       }
      }
     }
     /* Update all smoothed nodes in node_list */
     for( j = 0; j < num_nodes; j++ ) {
      node_list[j].coord[0] = smoo_list[j].coord[0] ;
      node_list[j].coord[1] = smoo_list[j].coord[1] ;
      node_list[j].coord[2] = smoo_list[j].coord[2] ;
     }
    }

/* 3.5 Release memory for smooth nodes vector */

    Msh3DFree( smoo_list ) ;
    Msh3DAdjElemFreeAll( ) ;

/* 3.6 Return smooth status */

    return(1) ;
}

/* -------------------------------------------------------------------
** Msh3DPushBdryFace - this routine pushes boundary faces onto the stack.
*/

static Msh3DBdryFace Msh3DPushBdryFace(
    Msh3DBdryNode   nodes,
    int             v1,
    int             v2,
    int             v3,
    int             use,
    int             layer )
{
    Msh3DBdryFace  face ;
    int            i, j ;

//fprintf (stderr,"markos debug, pushing face %d, %d, %d\n", v1, v2, v3) ;

    /* alloc memory for face and include nodes */

    face = Msh3DFaceAlloc() ;
    face->verts[0] = v1 ; face->verts[1] = v2 ; face->verts[2] = v3 ;

    /* set use for face:
     * 0 - geometry-based phase;
     * 1 - topology-based phase;
     * 2 - tracking-based phase. */

    face->use = use ;

    /* set layer for face:
     *  0 - face on initial boundary;
     * >0 - face created during mesh generation. */

    face->layer = layer ;

    /* initialize the two elements associated with this face:
     * rgn0 - first  element for face;
     * rgn1 - second element for face. */

    face->rgn[0] = -1 ;
    face->rgn[1] = -1 ;

    /* the center of the face is the algebraic mean of the corners */

    for ( i=0 ; i<3 ; i++ ) {
        face->center[i] = 0.0 ;
        for ( j=0 ; j<3 ; j++ ) {
            face->center[i] += nodes[face->verts[j]].coord[i] ;
        }
        face->center[i] /= 3.0 ;
    }

    /*  find the max and mins of the coordinates */

    for ( i=0 ; i<3 ; i++ ) {
        face->max[i] = nodes[v1].coord[i] ;
        face->min[i] = nodes[v1].coord[i] ;
        if (face->max[i] < nodes[v2].coord[i]) face->max[i] = nodes[v2].coord[i] ;
        if (face->min[i] > nodes[v2].coord[i]) face->min[i] = nodes[v2].coord[i] ;
        if (face->max[i] < nodes[v3].coord[i]) face->max[i] = nodes[v3].coord[i] ;
        if (face->min[i] > nodes[v3].coord[i]) face->min[i] = nodes[v3].coord[i] ;
    }

    /* compute the r and s vectors and cross products */

    for ( i=0 ; i<3 ; i++ ) {
        face->r[i] = nodes[v2].coord[i] - nodes[v1].coord[i] ;
        face->s[i] = nodes[v3].coord[i] - nodes[v1].coord[i] ;
        face->t[i] = nodes[v3].coord[i] - nodes[v2].coord[i] ;
    }

    face->r_x_s[0] = face->r[1] * face->s[2] - face->s[1] * face->r[2] ;
    face->r_x_s[1] = face->r[2] * face->s[0] - face->s[2] * face->r[0] ;
    face->r_x_s[2] = face->r[0] * face->s[1] - face->s[0] * face->r[1] ;

    face->rs_dot_v1 = face->r_x_s[0] * nodes[v1].coord[0] +
                      face->r_x_s[1] * nodes[v1].coord[1] +
                      face->r_x_s[2] * nodes[v1].coord[2] ;

    face->area = sqrt( face->r_x_s[0] * face->r_x_s[0] +
                       face->r_x_s[1] * face->r_x_s[1] +
                       face->r_x_s[2] * face->r_x_s[2] ) / 2.0 ;

    if (push_version == SMALL_VERSION)
     Msh3DBdryPushSmall( face ) ;
    else
     Msh3DBdryPushCorre( face ) ;

    return(face) ;
}

/* -------------------------------------------------------------------
** Msh3DPushPolyFace - this routine pushes polyhedron faces onto the stack.
*/

static Msh3DPolyFace Msh3DPushPolyFace( Msh3DBdryFace face )
{
    Msh3DPolyFace  poly_face ;

    poly_face = Msh3DPolyFaceAlloc() ;
    poly_face->face = face ;

    Msh3DPolyPush( poly_face ) ;

    return(poly_face) ;
}

/* -------------------------------------------------------------------
** Msh3DInsertNewNodes - these routines inserts new nodes.
*/

typedef struct _Msh3DId {
     int id ;
     int pos ;
     } Msh3DId ;

#if 0
static int Msh3DInsertNewNodesKer(Msh3DBdryNodeList nl)
{
 int                i, v1, v2, v3, cnt;
 Msh3DBdryFace      cur ;
 double             *newpt ;

 if (!bdry_stack) return 0 ;

 for ( cnt=0, cur=bdry_stack; cur != NULL; cnt++, cur=cur->next );

#if DSP_KER
 IniSimplex(cnt);

 cur = bdry_stack ;
 cnt = 0;
 while ( cur ) {
   v1 = cur->verts[0]; v2 = cur->verts[1]; v3 = cur->verts[2];
   AddFacetoTableau ( cnt,
                      nl[v1].coord[0], nl[v1].coord[1], nl[v1].coord[2],
                      nl[v2].coord[0], nl[v2].coord[1], nl[v2].coord[2],
                      nl[v3].coord[0], nl[v3].coord[1], nl[v3].coord[2] );
   cnt++;
   cur = cur->next ;
 }

 newpt = (double*)GetNewPoint ();
 fprintf( stderr, "\ncoords of new point: \nx = %f\ny = %f\nz = %f\n", newpt[0],
          newpt[1], newpt[2]);
#endif

 return 1 ;
}
#endif

static int Msh3DInsertNewNodesAdj (
 Msh3DBdryFace       face,
 int		    *n,
 int                 debug,
 int                *a,
 int                *face_pop,
 int                dsp )
{
 Msh3DBdryFace     facef ;
 Msh3DBdryFace     faceff ;
 int		   j ;
 int		   i ;
 int		   k ;
 int		   num_node,alloc ;
 int               num ;
 double            dot ;
 double		   node[3] ;
 double            new[3] ;
 double            cand_vec[3] ;

 /* write number of faces popped back before first try for this face */

#if DSP_PRNT
 if ( debug == 1 )
  fprintf(stderr, "New popped face \n") ;
 fprintf(stderr, "Trying insertion of new node\n" ) ;
#endif

 /* initiate baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initiate number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 /* add new coordinate for base face */

 Msh3DFindNode (face,debug,new);
 for (j = 0; j < 3; j++)
  node[j] += new[j];
 num++;

 /* add new coordinates for adjacent faces of base face */

 for (j = 0; j < 3; j++)
 {
  facef = Msh3DFindFace (face,face->verts[j],face->verts[(j+1)%3]);
  if (facef != NULL)
  {
   Msh3DFindNode (facef,debug,new);
   for (i = 0; i < 3; i++)
    node[i] += new[i];
   num++;
  }
 }

 /* evaluate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
  if (num_node >= node_alloc) {
   node_alloc += node_quantum ;
   node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
			 node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
  }
 }
 node_list[num_node-1].real_id = num_node-1 + int_offset;
 for (j = 0; j < 3 ; j++)
  node_list[num_node-1].coord[j] = node[j];
 node_list[num_node-1].active_flag = 1;
 node_list[num_node-1].faces  = (Msh3DAdjFaceList)0 ;
 node_list[num_node-1].ifaces = (Msh3DAdjIniFaceList)0 ;

 /* update number of nodes and alloc */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

#if DSP_NADJ
 if (FDEBUG && !GdbWait)
 {
  if (dot > 0.0)
  {
   if (dsp) {
    GdbClear() ;
    GdbSetCurFaceOn(face) ;
    GdbSetCurVertexOn(num_node-1) ;
    GdbDrawBdry() ;
   }
  }
 }
#endif

 if ( dot <= 0.0 ) return 0 ;
 else              return 1 ;
}

static int Msh3DInsertNewNodesBar (
 Msh3DBdryFace       face,
 int		    *n,
 int                 debug,
 int                *a,
 int                *face_pop )
{
 Msh3DBdryFace     facef ;
 int		   j ;
 int		   i ;
 int		   num_node ;
 int               num,alloc ;
 double            dot ;
 double		   node[3] ;
 double            new[3] ;
 double            cand_vec[3] ;

 /* initiate baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initiate number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 Msh3DTreeNode (face,debug,new);
 for (j = 0; j < 3; j++)
  node[j] += new[j];
 num++;

 /* evaluate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
  if (num_node >= node_alloc) {
   node_alloc += node_quantum ;
   node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
			     node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
  }
 }
 node_list[num_node-1].real_id = num_node-1 + int_offset;
 for (j = 0; j < 3 ; j++)
  node_list[num_node-1].coord[j] = node[j];
 node_list[num_node-1].active_flag = 1;
 node_list[num_node-1].faces  = (Msh3DAdjFaceList)0 ;
 node_list[num_node-1].ifaces = (Msh3DAdjIniFaceList)0 ;

 /* update number of nodes and allocation */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

#if DSP_NBAR
 if (FDEBUG && !GdbWait)
 {
  if (dot > 0.0)
  {
   GdbClear() ;
   GdbSetCurFaceOn(face) ;
   GdbSetCurVertexOn(num_node-1) ;
   GdbDrawBdry() ;
  }
 }
#endif

 if ( dot <= 0.0 ) return 0 ;
 else              return 1 ;
}

#if 0
static int Msh3DInsertNewNodesCen (
 Msh3DBdryFace       face,
 int		    *n,
 int                 debug,
 int                *a,
 int                *face_pop )
{
 Msh3DBdryFace     facef ;
 int		   j ;
 int		   i ;
 int		   num_node ;
 int               num,alloc ;
 double            dot ;
 double		   node[3] ;
 double            new[3] ;
 double            cand_vec[3] ;

 /* write number of faces popped back before first try for this face */

#if DSP_PRNT
 if ( face == poly_stack->face && debug == 1 )
 {
  fprintf(stderr, "New popped face \n") ;
  fprintf(stderr, "Trying insertion of new node\n" ) ;
 }
#endif

 /* initiate baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initiate number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 /* add new coordinate for baricent of base face */

#if 1
 Msh3DFindNode (face,debug,new);
 for (j = 0; j < 3; j++)
  node[j] += new[j];
 num++;
#else
 Msh3DTreeNode (face,debug,new);
 for (j = 0; j < 3; j++)
  node[j] += new[j];
 num++;
#endif

 /* evaluate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
  if (num_node >= node_alloc) {
   node_alloc += node_quantum ;
   node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
			     node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
  }
 }
 node_list[num_node-1].real_id = num_node-1 + int_offset;
 for (j = 0; j < 3 ; j++)
  node_list[num_node-1].coord[j] = node[j];
 node_list[num_node-1].active_flag = 1;
 node_list[num_node-1].faces  = (Msh3DAdjFaceList)0 ;
 node_list[num_node-1].ifaces = (Msh3DAdjIniFaceList)0 ;

 /* update number of nodes and allocation */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

#if 0
 if (FDEBUG && !GdbWait)
 {
  if (dot > 0.0)
  {
   GdbClear() ;
   GdbSetCurFaceOn(face) ;
   GdbSetCurVertexOn(num_node-1) ;
   GdbDrawBdry() ;
  }
 }
#endif

 if ( dot <= 0.0 ) return 0 ;
 else              return 1 ;
}
#endif

static int Msh3DInsertNewNodesPol (
 Msh3DBdryFace       face,
 int		    *n,
 int                 debug,
 int                *a,
 int                *face_pop,
 int                 dsp )
{
 Msh3DPolyFace     poly_face ;
 int               j ;
 int               i ;
 int		   num_node,alloc ;
 int               num ;
 double            dot ;
 double		   node[3] ;
 double            new[3] ;
 double            cand_vec[3] ;

 /* write message for try this face */

#if DSP_PRNT
 if ( debug == 0 )
  fprintf(stderr, "New popped face \n") ;
 fprintf(stderr, "Trying insertion of new node\n" ) ;
#endif

 /* initiate baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initiate number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 /* add new coordinates for faces of the polyhedron. If given debug
    is zero, the new coordinates for each face are their own nodes,
    else the new coordinates are the optimal nodes for each face
    based in given debug */

 Msh3DPolyReset ( ) ;
 while (poly_face = Msh3DPolyNext()) {
  if (poly_face != NULL)
  {
   if (debug) {
    Msh3DFindNode (poly_face->face,debug,new);
    for (i = 0; i < 3; i++)
     node[i] += new[i];
    num++;
   }
   else {
    for (j = 0; j < 3; j++) {
     for (i = 0; i < 3; i++)
      node[i] += node_list[poly_face->face->verts[j]].coord[i] ;
     num++ ;
    }
   }
  }
 }

 /* evaluate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

//fprintf (stderr,"markos debug, mid point is %f, %f, %f\n", node[0], node[1], node[2]) ;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
  if (num_node >= node_alloc) {
   node_alloc += node_quantum ;
   node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
			     node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
  }
 }
 node_list[num_node-1].real_id = num_node-1 + int_offset;
 for (j = 0; j < 3 ; j++)
  node_list[num_node-1].coord[j] = node[j];
 node_list[num_node-1].active_flag = 1;
 node_list[num_node-1].faces  = (Msh3DAdjFaceList)0 ;
 node_list[num_node-1].ifaces = (Msh3DAdjIniFaceList)0 ;

 /* update number of nodes and alloc */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

#if DSP_MESH
 if (FDEBUG && !GdbWait)
 {
  if (1 /* dot > 0.0 */)
  {
   if (dsp) {
    GdbClear() ;
    GdbSetCurFaceOn(face) ;
    GdbSetCurVertexOn(num_node-1) ;
    GdbDrawBdry() ;
   }
  }
 }
#endif

 if ( 0 /* dot <= 0.0 */ ) return 0 ;
 else                      return 1 ;
}

#if 0
static int Msh3DInsertNewNodesBdr (
 Msh3DBdryFace       face,
 int		    *n,
 int                 debug,
 int                *a,
 int                *face_pop,
 int                 dsp )
{
 Msh3DPolyFace     poly_face ;
 int               j ;
 int               i ;
 int               k ;
 int		   num_node,alloc ;
 int               num, v[3], bdry ;
 double            dot ;
 double		   node[3] ;
 double            new[3] ;
 double            cand_vec[3] ;

 /* write message for try this face */

#if DSP_PRNT
 if ( debug == 0 )
  fprintf(stderr, "New popped face \n") ;
 fprintf(stderr, "Trying insertion of new node\n" ) ;
#endif

 /* initiate baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initiate number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 /* add new coordinates for faces of the polyhedron. If given debug
    is zero, the new coordinates for each face are their own nodes,
    else the new coordinates are the optimal nodes for each face
    based in given debug */

 Msh3DPolyReset ( ) ;
 while (poly_face = Msh3DPolyNext()) {
  if (poly_face != NULL)
  {
   for (k = 0; k < 3; k++)
    v[k] = poly_face->face->verts[k] ;
   bdry = Msh3DBoundaryFace (poly_face->face, v) ;
   if (bdry) {
    Msh3DFindNode (poly_face->face,debug,new);
    for (i = 0; i < 3; i++)
     node[i] += new[i];
    num++;
   }
  }
 }

 /* evaluate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
  if (num_node >= node_alloc) {
   node_alloc += node_quantum ;
   node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
			     node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
  }
 }
 node_list[num_node-1].real_id = num_node-1 + int_offset;
 for (j = 0; j < 3 ; j++)
  node_list[num_node-1].coord[j] = node[j];
 node_list[num_node-1].active_flag = 1;
 node_list[num_node-1].faces  = (Msh3DAdjFaceList)0 ;
 node_list[num_node-1].ifaces = (Msh3DAdjIniFaceList)0 ;

 /* update number of nodes and alloc */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

#if DSP_MESH
 if (FDEBUG && !GdbWait)
 {
  if (1 /* dot > 0.0 */)
  {
   if (dsp) {
    GdbClear() ;
    GdbSetCurFaceOn(face) ;
    GdbSetCurVertexOn(num_node-1) ;
    GdbDrawBdry() ;
   }
  }
 }
#endif

 if ( 0 /* dot <= 0.0 */ ) return 0 ;
 else                      return 1 ;
}
#endif

#if 0
static void Msh3DInsertNewNodesOct (int *n)
{
 Msh3DBdryFace     face ;
 Msh3DId          *aux ;
 int		   id ;
 int		   i ;
 int		   j ;
 int		   k ;
 int		   num_face ;
 int		   num_node ;
 int		  *faces ;
 int		   num_gen_nodes = 0;
 double           *nodes ;
 double		  *generated_nodes ;
 FILE		  *fp;

 /* initiate number of nodes */

 num_node = *n;

 /* find the number of faces */

 num_face = 0;
 Msh3DBdryReset ( );
 while ( face = Msh3DBdryNext() )
  num_face++;

 /* fill nodes and faces to find new internal points */

 aux   = (Msh3DId *)Msh3DMalloc( num_node * 3 * sizeof(Msh3DId) ) ;
 nodes = (double *)Msh3DMalloc( num_node * 3 * sizeof(double) ) ;
 faces = (int *)Msh3DMalloc( num_face * 3 * sizeof(int) ) ;
 num_face = 0;
 num_node = 0;
 Msh3DBdryReset ( );
 while ( face = Msh3DBdryNext() ) {
   for (j = 0; j < 3; j++) {
    Msh3DPushNode (aux,nodes,faces,&num_node,num_face,face->verts[j],j);
   }
   num_face++;
 }

 /* put those new nodes in node_list */

 num_node += num_gen_nodes;
 if ((*n+num_node) >= node_alloc) {
  node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
		(*n+num_node) * sizeof(Msh3DBdryNodeRec) ) ;
 }
 for (k = 0,i = *n; k < num_gen_nodes; i++,k++) {
  node_list[i].real_id = i + int_offset;
  for (j = 0; j < 3 ; j++)
   node_list[i].coord[j] = generated_nodes[k*3+j];
  node_list[i].active_flag = 1;
  node_list[i].faces  = (Msh3DAdjFaceList)0 ;
  node_list[i].ifaces = (Msh3DAdjIniFaceList)0 ;
 }

 /* update the number of nodes */

 *n += num_gen_nodes;
 nintnode += num_gen_nodes;
}
#endif

typedef struct _Msh3Dface {
 int    flag;
 int    vnodes[3];
} Msh3Dface ;

static int Msh3DElemPoly(
 int                ielem,
 int                *nelems,
 int                *elems,
 int                *nfaces,
 int                faces[][3],
 int                step )
{
 Msh3DBdryFaceRec   new ;
 int                node_indx ;
 double             ind_vol = 0.0 ;
 double             ind_rms = 0.0 ;
 double             ind_sha = 0.0 ;
 int                i, j, k, m, nid, sliver, found, add, fac = 0;
 int                v0, v1, v2;
 int                p0, p1, p2;
 int                annodes = 0, acnodes = MSH3D_ELEM_QUANTUM ;
 int                pnnodes = 0, pcnodes = MSH3D_ELEM_QUANTUM ;
 int                snnodes = 0, scnodes = MSH3D_ELEM_QUANTUM ;
 int                tnnodes = 0, tcnodes = MSH3D_ELEM_QUANTUM ;
 int                anfaces = 0, acfaces = MSH3D_ELEM_QUANTUM ;
 int                *anodes ;
 int                *pnodes ;
 int                *snodes ;
 int                *tnodes ;
 Msh3Dface          *afaces ;

 /* return if the element was already deleted in former step */

 if( (elems[ielem*4+0] == -1) &&
     (elems[ielem*4+1] == -1) &&
     (elems[ielem*4+2] == -1) &&
     (elems[ielem*4+3] == -1) )
   return 0 ;

 /* evaluate shape of element. If the shape isn't sliver for the step, just
    return */

 new.verts[0] = elems[ielem*4+0] ;
 new.verts[1] = elems[ielem*4+1] ;
 new.verts[2] = elems[ielem*4+2] ;
 node_indx    = elems[ielem*4+3] ;
 ind_vol = Msh3DVolum( &new, node_list, node_indx ) ;
 ind_rms = Msh3DSrms( &new, node_list, node_indx ) ;
 ind_sha = (ind_rms * ind_rms * ind_rms) / (ind_vol) ;

//fprintf (stderr,"markos debug, back tracking, elem = %d, ind_sha = %f, limits = [%f, %f]\n", ielem, ind_sha, (SHA_FACTOR / (MUL_FACTOR+(05*(step-1)))), (SHA_FACTOR * (MUL_FACTOR+(05*(step-1))))) ;
//fprintf (stderr,"markos debug, elem = %d is (%d, %d, %d, %d)\n", ielem, elems[4*ielem+0], elems[4*ielem+1], elems[4*ielem+2], elems[4*ielem+3]);

 if( (ind_sha > (SHA_FACTOR / (MUL_FACTOR+(05*(step-1)))) ) &&
     (ind_sha < (SHA_FACTOR * (MUL_FACTOR+(05*(step-1)))) ) )
  return 0 ;

//fprintf (stderr,"markos debug, back tracking, elem = %d will be improved by back tracking\n", ielem) ;

 /* allocate elements to be deleted in the back-tracking process */

 if( adj_elem_dele != 0 )
  Msh3DFree( adj_elem_dele ) ;
 num_elem_deleted = 0 ;
 num_elem_created = 0 ;
 adj_elem_dele = (Msh3DAdjElem)Msh3DMalloc( 10 * Msh3D_DELETE_MEMO *
                       sizeof(Msh3DAdjElemRec) ) ;

 /* allocate memory for local vectors */

 anodes = (int *)Msh3DMalloc( acnodes*sizeof(int) ) ;
 pnodes = (int *)Msh3DMalloc( pcnodes*sizeof(int) ) ;
 snodes = (int *)Msh3DMalloc( scnodes*sizeof(int) ) ;
 tnodes = (int *)Msh3DMalloc( tcnodes*sizeof(int) ) ;
 afaces = (Msh3Dface *)Msh3DMalloc( acfaces*sizeof(Msh3Dface) ) ;

 /* adjust tolerance for the step */

 tol_inters = 1.0e-3 ;

 /* find nodes adjacent of sliver element */

 for( i = 0; i < (*nelems); i++ ) {
  found = 0 ;
  for( j = 0; j < 4; j++ ) {
   nid = elems[ielem*4+j] ;
   if( (elems[i*4+0] == nid) ||
       (elems[i*4+1] == nid) ||
       (elems[i*4+2] == nid) ||
       (elems[i*4+3] == nid) )
   {
    found++ ;
   }
  }
  if( found == 3 ) {
   for (j = 0; j < 4; j++) {
    found = 0;
    nid = elems[i*4+j] ;
    for (k = 0; k < annodes; k++)
    {
     if (nid == anodes[k])
     {
      found = 1 ;
      break ;
     }
    }
    if( !found ) {
     if( annodes > acnodes ) {
      acnodes += MSH3D_ELEM_QUANTUM ;
      anodes = (int *)Msh3DRealloc( anodes, acnodes*sizeof(int) ) ;
     }
     anodes[annodes++] = nid;

     //fprintf (stderr,"markos debug, node = %d\n", nid) ;
     //fprintf (stderr,"markos debug, elem = %d\n", i) ;
    }
   }
  }
 }

 /* find nodes adjacent of adjacent sliver element */

 tnnodes = (nbdrynode + nintnode) ;
 if( tnnodes > tcnodes ) {
  tcnodes += tnnodes ;
  tnodes = (int *)Msh3DRealloc( tnodes, tcnodes*sizeof(int) ) ;
 }
 for (m = 0; m < (nbdrynode + nintnode); m++) {
  tnodes[m] = 0 ;
 }
 for (i = 0; i < (*nelems); i++)
 {
  found = 0;
  for (m = 0; m < 4; m++)
  {
   for (j = 0; j < annodes; j++)
   {
    nid = anodes[j];
    if( elems[i*4+m] == nid )
    {
     found++;
     break;
    }
   }
  }
  if (found == 3)
  {
   for (m = 0; m < 4; m++) {
    tnodes[elems[i*4+m]] += 1 ;
   }
  }
 }
 for (m = 0; m < (nbdrynode + nintnode); m++)
 {
  if (tnodes[m] >= 2) {
   if( snnodes > scnodes ) {
    scnodes += MSH3D_ELEM_QUANTUM ;
    snodes = (int *)Msh3DRealloc( snodes, scnodes*sizeof(int) ) ;
   }
   snodes[snnodes++] = m ;
  }
 }
 for (i = 0; i < snnodes; i++)
 {
  nid = snodes[i];
  found = 0;
  for (k = 0; k < annodes; k++)
  {
   if (nid == anodes[k])
   {
    found = 1;
    break;
   }
  }
  if (!found) {
   if( annodes > acnodes ) {
    acnodes += MSH3D_ELEM_QUANTUM ;
    anodes = (int *)Msh3DRealloc( anodes, acnodes*sizeof(int) ) ;
   }
   anodes[annodes++] = nid;

   //fprintf (stderr,"markos debug, add node = %d\n", nid) ;
  }
 }

 /* find faces adjacent of sliver element */

 for (i = 0; i < (*nelems); i++)
 {
  found = 0;
  for (m = 0; m < 4; m++)
  {
   for (j = 0; j < annodes; j++)
   {
    nid = anodes[j];
    if( elems[i*4+m] == nid )
    {
     found++;
     break;
    }
   }
  }

  if (found == 4)
  {
   for (j = 0; j < 4; j++)
   {
    found = 0;
    if (j == 0)
    {
     v0 = elems[i*4+0] ;
     v1 = elems[i*4+1] ;
     v2 = elems[i*4+2] ;
    }
    if (j == 1)
    {
     v0 = elems[i*4+0] ;
     v1 = elems[i*4+3] ;
     v2 = elems[i*4+1] ;
    }
    if (j == 2)
    {
     v0 = elems[i*4+0] ;
     v1 = elems[i*4+2] ;
     v2 = elems[i*4+3] ;
    }
    if (j == 3)
    {
     v0 = elems[i*4+1] ;
     v1 = elems[i*4+3] ;
     v2 = elems[i*4+2] ;
    }
    for (k = 0; k < annodes; k++)
    {
     if ((v0 == anodes[k]) || (v1 == anodes[k]) || (v2 == anodes[k]))
      found++;
    }
    if (found == 3)
    {
     for (k = 0; k < anfaces; k++)
     {
      fac = 0;
      for (m = 0; m < 3; m++)
      {
       if ((v0 == afaces[k].vnodes[m]) ||
           (v1 == afaces[k].vnodes[m]) ||
           (v2 == afaces[k].vnodes[m]))
        fac++;
      }
      if (fac == 3) break;
     }
     if (fac == 3)
     {
      afaces[k].flag = 0;
     }
     else
     {
      if( anfaces > acfaces ) {
       acfaces += MSH3D_ELEM_QUANTUM ;
       afaces = (Msh3Dface *)Msh3DRealloc( afaces, acfaces*sizeof(Msh3Dface) ) ;
      }
      afaces[anfaces].flag = 1;
      afaces[anfaces].vnodes[0] = v0;
      afaces[anfaces].vnodes[1] = v1;
      afaces[anfaces].vnodes[2] = v2;
      anfaces++;
     }
    }
   }

   if( num_elem_deleted < Msh3D_DELETE_MEMO )
   {
    adj_elem_dele[num_elem_deleted].elem = i ;
    adj_elem_dele[num_elem_deleted].verts[0] = elems[i*4+0] ;
    adj_elem_dele[num_elem_deleted].verts[1] = elems[i*4+1] ;
    adj_elem_dele[num_elem_deleted].verts[2] = elems[i*4+2] ;
    adj_elem_dele[num_elem_deleted].verts[3] = elems[i*4+3] ;
    num_elem_deleted++ ;
   }

   //fprintf (stderr,"markos debug, old element %d, (%d, %d, %d, %d)\n", i, elems[4*i+0], elems[4*i+1], elems[4*i+2], elems[4*i+3]);

   for (j = 0; j < 4; j++)
    elems[i*4+j] = -1 ;
  }
 }

 /* update nodes */

 for (i = 0; i < annodes; i++)
 {
  found = 0 ;
  for (j = 0; j < anfaces; j++)
  {
   if (afaces[j].flag)
   {
    if ((anodes[i] == afaces[j].vnodes[0]) ||
        (anodes[i] == afaces[j].vnodes[1]) ||
        (anodes[i] == afaces[j].vnodes[2]))
    {
     if( pnnodes > pcnodes ) {
      pcnodes += MSH3D_ELEM_QUANTUM ;
      pnodes = (int *)Msh3DRealloc( pnodes, pcnodes*sizeof(int) ) ;
     }
     pnodes[pnnodes++] = anodes[i];
     found = 1;
     break;
    }
   }
  }
  if( !found )
   node_list[anodes[i]].active_flag = 0 ;
  else
   node_list[anodes[i]].active_flag = 1 ;
 }

 /* update elements */

 *nfaces = 0 ;
 for (i = 0; i < anfaces; i++)
 {
  if (afaces[i].flag)
  {
   faces[(*nfaces)][0] = afaces[i].vnodes[0] ;
   faces[(*nfaces)][1] = afaces[i].vnodes[1] ;
   faces[(*nfaces)][2] = afaces[i].vnodes[2] ;
   (*nfaces)++ ;
  }
 }

 /* free the allocated memory */

 Msh3DFree( anodes ) ;
 Msh3DFree( pnodes ) ;
 Msh3DFree( snodes ) ;
 Msh3DFree( tnodes ) ;
 Msh3DFree( afaces ) ;

 /* if we get here, the element is sliver and will be corrected */

 return 1 ;
}

static int Msh3DStarPoly (
  int                *nnodes,
  int                *elem,
  int                *nelem,
  int                node_poly )
{
 Msh3DBdryFace      face, bdry_face, visi = 0 ;
 Msh3DPolyFace      poly, poly_face ;
 int                i, j, k, found, vert, bdry_vert ;
 int                v[3], check, alloc = 1, face_pop = 0 ;

 /* find a suitable criteria for when use boundary contraction instead of point in centroid later */

 return 1 ;

 /* If there's already a visible node then no test is necessary */

 if( node_poly != -1 ) return 1 ;

 /* Initialize polyhedron faces to be considered */

 poly = poly_tail ;

 /* Get the new faces introduced in polyhedron. Test if the three last faces in the
    polyhedron, what represents the faces that were introduced by CrtnFDelF, are
    crack faces. */

 for( i = 0; i < 3; i++ )
 {
  /* Get the new faces introduced in polyhedron */

  if( poly ) {
   face = poly->face ;
   poly = poly->prev ;
  }
  if( !face ) continue ;

  /* Verify if a crack face is found. If it's, then this polyhedron can't be
     star-shaped */

  Msh3DPolyReset () ;
  while (poly_face = Msh3DPolyNext ()) {
   bdry_face = poly_face->face ;
   if (face != bdry_face) {
    found = 0 ;
    for( j = 0; j < 3; j++ ) {
     vert = face->verts[j] ;
     for( k = 0; k < 3; k++ ) {
      bdry_vert = bdry_face->verts[k] ;
      if( (node_list[vert].coord[0] == node_list[bdry_vert].coord[0]) &&
          (node_list[vert].coord[1] == node_list[bdry_vert].coord[1]) &&
          (node_list[vert].coord[2] == node_list[bdry_vert].coord[2])  )
      {
       found++ ;
       break ;
      }
     }
    }
    if( found == 3 )
     return 0 ;
   }
  }

 }

 /* Verify if the unvisible face, if exists, doesn't have any element
    associated with, which represents that it reached another polyhedron or
    the original boundary */

 poly = poly_stack ;
 for (k = 0; k <= POL_NODES; k++)
 {
  check = Msh3DInsertNewNodesPol (poly->face, nnodes, k, &alloc, &face_pop, 1) ;
  if (check)
   check = Msh3DCheckPoly( node_list, (*nnodes)-1, -1, 1, elem,
                           nelem, &visi, 1 ) ;
  if (check)
   check = Msh3DCheckVisib( (*nnodes)-1, &visi ) ;
  if (check) break ;
 }
 (*nnodes)-- ;
 nintnode-- ;
 if (!visi) visi = poly_stack->face ;
 if (!check)
 {
  for ( i = 0; i < 3; i++ )
   v[i] = visi->verts[i] ;
  if( Msh3DNoElemFace (visi, v, elem, nelem) )
   return 0 ;
 }

 /* if we get here no crack faces were found. */

 return 1 ;
}

static int Msh3DVisiPoly (
  int                *nnodes,
  int                *nelems,
  int                **elems,
  int                step,
  int                pjmesh_use_box_test )
{
  Msh3DPolyFace      poly, poly_face, cur, inter ;
  Msh3DBdryFace      face, visi, bdry_face ;
  Msh3DBdryFaceRec   new ;
  double             ind_vol = 0.0 ;
  double             ind_rms = 0.0 ;
  double             ind_sha = 0.0 ;
  int                k, node_indx, valid, check, alloc = 1, face_pop = 0 ;
  int                nface_node = 0 ;
  int                nface_face = 0 ;
  int                face_node[100] ;
  Msh3DBdryFace      face_face[100] ;

 /* find a suitable criteria later */

 return 1 ;

 /* find polyhedron */

 face = bdry_stack ;
 Msh3DFindPoly (face, face_face, face_node, &nface_face, &nface_node, 1, pjmesh_use_box_test) ;
 poly = poly_stack ;

 /* verify if this poyhedron is star-shaped */

 for (k = 0; k <= POL_NODES; k++)
 {
  /* node inserted */

  check = Msh3DInsertNewNodesPol (poly->face, nnodes, k, &alloc, &face_pop, 1) ;

  /* check validity for node inserted */

  if (check)
   check = Msh3DCheckPoly( node_list, (*nnodes)-1, -1, 1, *elems,
                           nelems, &visi, 1 ) ;
  if (check)
   check = Msh3DCheckVisib( (*nnodes)-1, &visi ) ;

  /* it's not necessary try anymore if node is valid */

  if (check) break ;
 }

 /* verify if the star-shaped polyhedron will create good elements */

 if (0 && check) {
  Msh3DPolyReset() ;
  while ( poly_face = Msh3DPolyNext() ) {
   bdry_face = poly_face->face ;
   new.verts[0] = bdry_face->verts[0] ;
   new.verts[1] = bdry_face->verts[1] ;
   new.verts[2] = bdry_face->verts[2] ;
   node_indx    = (*nnodes) - 1 ;
   ind_vol = Msh3DVolum( &new, node_list, node_indx ) ;
   ind_rms = Msh3DSrms( &new, node_list, node_indx ) ;
   ind_sha = (ind_rms * ind_rms * ind_rms) / (ind_vol) ;
   if( (ind_sha > (SHA_FACTOR / (MUL_FACTOR+(05*(step-1)))) ) &&
       (ind_sha < (SHA_FACTOR * (MUL_FACTOR+(05*(step-1)))) ) )
   {
    continue ;
   }
   else
   {
    check = 0 ;
    break ;
   }
  }
 }

 /* release polyhedron because a temporary one was created just to check */

 Msh3DPolyReset() ;
 while ( poly_face = Msh3DPolyNext() ) {
  bdry_face = poly_face->face ;
  Msh3DPolyDelete( bdry_face ) ;
 }
 poly_stack = poly_tail = NULL ;

 /* update nnodes and nintnode because a temporary node was created just
    to make the check */

 (*nnodes)-- ;
 nintnode-- ;

 /* set validity for node inserted */

 valid = check ;
 if (valid) {
  return 1 ;
 }
 else {
  return 0 ;
 }
}

/* -------------------------------------------------------------------
** Msh3DFindPoly - these routines manage a find a polyhedron, given
**                 an initial face that belongs to it. It treats all
**                 pathological cases related to nodes and edges,
**                 that is, two polyhedra linked by only a node or
**                 an edge are split in two, to be treated separately.
*/

static void Msh3DFindPolyNodes (
 Msh3DBdryFace       face,
 Msh3DBdryFace       *face_face,
 int                 *face_node,
 int                 *nface_face,
 int                 *nface_node,
 int                 pjmesh_use_box_test )
{
 Msh3DPolyFace       cur_poly, face_poly ;
 Msh3DAdjFace        cur_face ;
 int                 i, j, ve, found_face, found_node ;

 /* insert given face if it's not already in polyhedron faces */

 found_face = 0 ;
 for (i = 0; i < (*nface_face); i++) {
  if (face_face[i] == face) {
   found_face = 1 ;
   break ;
  }
 }

 /* PJmesh - if face is a remaining, it should be added to polyhedron only
	if all its three vertices belong to the polyhedron */

	if (pjmesh_use_box_test)
	{
		if ((!found_face) && (face->use == -1))
		{
			found_node = 0;

			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < (*nface_node); j++)
				{
					if (face_node[j] == face->verts[i])
					{
						found_node++;
					}
				}
			}

			if (found_node == 3)
			{
				PMsh3DInvertFace(face);
			}
			else
			{
				found_face = 1;
			}
		}
	}

/* end PJmesh - if face is a remaining, it should be added to polyhedron only
	if all its three vertices belong to the polyhedron */

 if (!found_face) {
  face_face[(*nface_face)++] = face ;
 }

 /* traverse recursively the face nodes to complete the polyhedron faces */

 if (!found_face) {
  for (i = 0; i < 3; i++) {
   ve = face->verts[i] ;
   found_node = 0 ;
   for (j = 0; j < (*nface_node); j++) {
    if (face_node[j] == ve) {
     found_node = 1 ;
     break ;
    }
   }
   if (!found_node) {
    face_node[(*nface_node)++] = ve ;
    for (cur_face = node_list[ve].faces; cur_face; cur_face = cur_face->next)
     Msh3DFindPolyNodes (cur_face->face, face_face, face_node, nface_face, nface_node, pjmesh_use_box_test) ;
   }
  }
 }
}

static void Msh3DFindPolyBuild (
 Msh3DBdryFace       face,
 int                 *face_node,
 int                 *nface_node,
 int                 shp,
 int                 pjmesh_use_box_test)
{
 Msh3DPolyFace       cur_poly, face_poly ;
 Msh3DBdryFace       cur_face, real_face[10000] ;
 Msh3DAdjFace        cur, oth, can, nxt ;
 Msh3DBdryFace       fid, nid ;
 int                 i, j, k, l, ve, use_face, found_node, new_node = 1 ;
 int                 m, n, v1, v2, num, p, q, not_ok, face_num ;
 int                 real_node[10000], nreal_node = 0, nreal_face = 0 ;
 int                 adj_faces_with_two_found_nodes ;
 int                 adj_faces_to_this_edge ;
 int                 edge_with_less_than_two_faces ;
 int                 in_num_edge, *in_edges, edge_sta, edge_end, found ;

//added by markos
 int pjmesh_found_useNeg_face = 0;
//end added by markos

#if OLD_POLY
 /* insert given face nodes in real_nodes vector */

 for (i = 0; i < 3; i++) {
  ve = face->verts[i] ;
  found_node = 0 ;
  for (j = 0; j < nreal_node; j++) {
   if (real_node[j] == ve) {
    found_node = 1 ;
    break ;
   }
  }
  if (!found_node) {
   real_node[nreal_node++] = ve ;
  }
 }

 /* introduce in real_nodes vector only the face_nodes that have at least
    one face with two nodes in real_nodes, which represents that this node
    is in the same side of real_nodes introduced before, to avoid cases of
    non-manifold polyhedra. This process stops when no new node is left in
    face_nodes vector to be introduced in real_nodes vector. */

 while( new_node ) {

  /* initiate flag for new_node introduced */

  new_node = 0 ;

  /* build the polyhedron based in real_nodes vector */

  for (i = 0; i < (*nface_node); i++) {

   /* look at all adjacent faces of the node */

   for (cur = node_list[face_node[i]].faces; cur; cur = cur->next) {

    /* verify if the face has at least two real_nodes different of current
       node, which represents that this node should be introduced in
       real_nodes vector, if it wasn't before. */

    found_node = 0 ;
    for( j = 0; j < 3; j++ ) {
     if( cur->face->verts[j] == face_node[i] ) continue ;
     for( k = 0; k < (*nface_node); k++) {
      if( cur->face->verts[j] == real_node[k] ) {
       found_node++ ;
       break ;
      }
     }
    }
    if( found_node == 2 ) {
     for (k = 0; k < 3; k++) {
      ve = cur->face->verts[k] ;
      found_node = 0 ;
      for (j = 0; j < nreal_node; j++) {
       if (real_node[j] == ve) {
        found_node = 1 ;
        break ;
       }
      }
      if (!found_node) {
       real_node[nreal_node++] = ve ;
       new_node = 1 ;
      }
     }
    }

   }
  }
 }

 /* now build the polyhedron stack based in real polyhedron nodes. For a
    face that is in boundary stack be introduced in polyhedron stack, it
    should have all three nodes as real nodes */

 Msh3DBdryReset () ;
 while (cur_face = Msh3DBdryNext ()) {
  use_face = 0 ;
  for (i = 0; i < 3; i++) {
   for (j = 0; j < nreal_node; j++) {
    if (real_node[j] == cur_face->verts[i]) {
     use_face++ ;
     break ;
    }
   }
  }
  if (use_face == 3) {
   face_poly = Msh3DPushPolyFace( cur_face ) ;
  }
 }
#else
 /* insert given face nodes in real_nodes vector */

 for (i = 0; i < 3; i++) {
  ve = face->verts[i] ;
  found_node = 0 ;
  for (j = 0; j < nreal_node; j++) {
   if (real_node[j] == ve) {
    found_node = 1 ;
    break ;
   }
  }
  if (!found_node) {
   real_node[nreal_node++] = ve ;
  }
 }

 /* insert also nodes belonging to the adjacent faces of the given face
  *  nodes, if faces adjacent to the three edges are smaller than 2. */

 for (i = 0; i < 3; i++) {
  /* get the edge */
  v1 = face->verts[i] ;
  v2 = face->verts[(i+1)%3] ;
  adj_faces_to_this_edge = 0;
  /* look how many adjacent faces this edge has */
  for (cur = node_list[v1].faces; cur; cur = cur->next) {
   fid = cur->face ;
   for (nxt = node_list[v2].faces; nxt; nxt = nxt->next) {
    nid = nxt->face ;
    if (fid == nid) adj_faces_to_this_edge++;
   }
  }
  /* treat the case when two equal faces, with reversed orientations,
   * are on the polyhedron. If this happens, we should not consider
   * these faces and so we should update the number of adjcent faces.
   * This is a pathological case and should rarely happens.  */
  for (cur = node_list[v1].faces; cur; cur = cur->next) {
   fid = cur->face ;
   for (nxt = node_list[v2].faces; nxt; nxt = nxt->next) {
    nid = nxt->face ;
    num = 0 ;
    for (p = 0; p < 3; p++) {
     for (q = 0; q < 3; q++) {
      if (fid->verts[p] == nid->verts[q]) {
       num++;
      }
     }
    }
    if (num == 3) {
     if (fid != nid) {
      adj_faces_to_this_edge--;
     }
    }
   }
  }
  /* if the edge has two or less adjacent faces, include in the
   * real_node vector all nodes belonging to these faces */
  if (adj_faces_to_this_edge <= 2) {
   /* include nodes for v1-v2 edge */
   for (cur = node_list[v1].faces; cur; cur = cur->next) {
    fid = cur->face ;
    for (nxt = node_list[v2].faces; nxt; nxt = nxt->next) {
     nid = nxt->face ;
     if (fid == nid) {
      for( j = 0; j < 3; j++ ) {
       if (fid->verts[j] == v1) continue ;
       if (fid->verts[j] == v2) continue ;
       ve = fid->verts[j] ;
       found_node = 0 ;
       for (k = 0; k < nreal_node; k++) {
        if (real_node[k] == ve) {
         found_node = 1 ;
         break ;
        }
       }
       if (!found_node) {
        real_node[nreal_node++] = ve ;
       }
      }
     }
    }
   }
  }
 }

 /* introduce in real_nodes vector only the face_nodes that have at least
    one face with two nodes in real_nodes, which represents that this node
    is in the same side of real_nodes introduced before, to avoid cases of
    non-manifold polyhedra. This process stops when no new node is left in
    face_nodes vector to be introduced in real_nodes vector. */

 while( new_node ) {

  /* initiate flag for new_node introduced */

  new_node = 0 ;

  /* build the polyhedron based in real_nodes vector */

  for (i = 0; i < (*nface_node); i++) {

   /* look at all adjacent faces of the node */

   adj_faces_with_two_found_nodes = 0 ;
   for (cur = node_list[face_node[i]].faces; cur; cur = cur->next) {

    /* verify if the face has at least two real_nodes different of current
       node, which represents that this node should be introduced in
       real_nodes vector, if it wasn't before. */

    found_node = 0 ;
    for( j = 0; j < 3; j++ ) {
     if( cur->face->verts[j] == face_node[i] ) continue ;
     for( k = 0; k < (nreal_node); k++) {
      if( cur->face->verts[j] == real_node[k] ) {
       found_node++ ;
       break ;
      }
     }
    }
    if( found_node == 2 ) {
     adj_faces_with_two_found_nodes++ ;
    }
   }

   /* If we face the node situation (found_node==2) for more than one
    * face (two or more) for this node then we do the normal procedure. */

   if (adj_faces_with_two_found_nodes >= 2) {

    /* look at all adjacent faces of the node */

    for (cur = node_list[face_node[i]].faces; cur; cur = cur->next) {

     /* verify if the face has at least two real_nodes different of current
        node, which represents that this node should be introduced in
        real_nodes vector, if it wasn't before. */

     found_node = 0 ;
     for( j = 0; j < 3; j++ ) {
      if( cur->face->verts[j] == face_node[i] ) continue ;
      for( k = 0; k < (nreal_node); k++) {
       if( cur->face->verts[j] == real_node[k] ) {
        found_node++ ;
        break ;
       }
      }
     }
     if( found_node == 2 ) {
      for (k = 0; k < 3; k++) {
       ve = cur->face->verts[k] ;
       found_node = 0 ;
       for (j = 0; j < nreal_node; j++) {
        if (real_node[j] == ve) {
         found_node = 1 ;
         break ;
        }
       }
       if (!found_node) {
        real_node[nreal_node++] = ve ;
        new_node = 1 ;
       }
      }
     }
    }
   }
  }
 }

/* PJMesh - if the face to be back-tracked is adjacent to a useNeg face (that
		corresponds to an output face, that will be advanced later), then that
		face is recursively adjacent to (perhaps) all the useNeg faces. thus,
		the center point of the polyhedron is likely to be not-visible to
		several faces and, therefore will delete most of the elements created
		by the advancing front algorithm. therefore, this face is also marked
		as a useNeg face, and this back-tracking is canceled. */ /*unused

	if (pjmesh_use_box_test)
	{
	 Msh3DBdryReset () ;
	 while (cur_face = Msh3DBdryNext ()) {
	  use_face = 0 ;
	  for (i = 0; i < 3; i++) {
	   for (j = 0; j < nreal_node; j++) {
		if (real_node[j] == cur_face->verts[i]) {
		 use_face++ ;
		 break ;
		}
	   }
	  }
	  if ((use_face == 3) && (cur_face->use == -1)) {
	   pjmesh_found_useNeg_face = 1;
	   break;
	  }
	 }

	 if (pjmesh_found_useNeg_face)
	 {
		 Msh3DBdryReset () ;
		 while (cur_face = Msh3DBdryNext ()) {
		  use_face = 0 ;
		  for (i = 0; i < 3; i++) {
		   for (j = 0; j < nreal_node; j++) {
			if (real_node[j] == cur_face->verts[i]) {
			 use_face++ ;
			 break ;
			}
		   }
		  }
		  if ((use_face == 3) && (cur_face->use == 2)) {
		   cur_face->use = -1;
		   if (cur_face->use == -1) cur_face->layer = 0;
		  }
		 }

		 return;
	 }
	}*/

/* end PJMesh - if the face to be back-tracked is adjacent to a useNeg face (that
		corresponds to an output face, that will be advanced later), then that
		face is recursively adjacent to (perhaps) all the useNeg faces. thus,
		the center point of the polyhedron is likely to be not-visible to
		several faces and, therefore will delete most of the elements created
		by the advancing front algorithm. therefore, this face is also marked
		as a useNeg face, and this back-tracking is canceled. */

 /* now build the polyhedron stack based in real polyhedron nodes. For a
    face that is in boundary stack be introduced in polyhedron stack, it
    should have all three nodes as real nodes */

 Msh3DBdryReset () ;
 while (cur_face = Msh3DBdryNext ()) {
  use_face = 0 ;
  for (i = 0; i < 3; i++) {
   for (j = 0; j < nreal_node; j++) {
    if (real_node[j] == cur_face->verts[i]) {
     use_face++ ;
     break ;
    }
   }
  }
  if (use_face == 3) {
   face_poly = Msh3DPushPolyFace( cur_face ) ;
  }
 }

 /* here we have to check if all edges have at least two adjacent faces,
    otherwise some nodes were left behind and need to be included. This
    happens in pathological cases where faces are inside the polyhedron. */

 not_ok = 1 ;
 while (not_ok) {
  /* allocate memory for edges */
  face_num = 0 ;
  Msh3DPolyReset () ;
  while (cur_poly = Msh3DPolyNext ()) {
   face_num++ ;
  }

  in_num_edge = nreal_node + face_num - 2;  /* Euler number */
  in_num_edge += 100;
  in_edges = (int *) Msh3DMalloc (in_num_edge * 3 * sizeof (int));
  /* find the number of adjacent faces for each edge on polyhedron */
  in_num_edge = 0;
  Msh3DPolyReset () ;
  while (cur_poly = Msh3DPolyNext ()) {
   fid = cur_poly->face ;
   for (k = 0; k < 3; k++) {
    edge_sta = fid->verts[(k%3)] ;
    edge_end = fid->verts[((k+1)%3)] ;
    found = 0 ;
    for (j = 0; j < in_num_edge; j++) {
     if ((in_edges[j*3+0]==edge_sta && in_edges[j*3+1]==edge_end) ||
	 (in_edges[j*3+0]==edge_end && in_edges[j*3+1]==edge_sta)) {
      in_edges[j*3+2]++ ;
      found = 1 ;
      break ;
     }
    }
    if (!found) {
     in_edges[in_num_edge*3+0] = edge_sta ;
     in_edges[in_num_edge*3+1] = edge_end ;
     in_edges[in_num_edge*3+2] = 1 ;
     in_num_edge++ ;
    }
   }
  }
  /* if we have edges with less than one adjacent face, include the other
     node, related to the other face of the polyhedron that is missing. */
  edge_with_less_than_two_faces = 0;
  for (i = 0; i < in_num_edge; i++) {
   if (in_edges[i*3+2] < 2) {
    /* check in all faces to include node for face that is missing. */
    Msh3DBdryReset () ;
    while (cur_face = Msh3DBdryNext ()) {
     /* check if this face has the edge */
     found = 0 ;
     for (k = 0; k < 3; k++) {
      edge_sta = cur_face->verts[(k%3)] ;
      edge_end = cur_face->verts[((k+1)%3)] ;
      if ((in_edges[i*3+0]==edge_sta && in_edges[i*3+1]==edge_end) ||
	  (in_edges[i*3+0]==edge_end && in_edges[i*3+1]==edge_sta)) {
       found = 1 ;
       break ;
      }
     }
     /* if the face has the edge, insert all nodes into real_nodes,
        if they are not already there. */
     if (found) {
      for (k = 0; k < 3; k++) {
       ve = cur_face->verts[k] ;
       found_node = 0 ;
       for (j = 0; j < nreal_node; j++) {
        if (real_node[j] == ve) {
         found_node = 1 ;
	 break ;
        }
       }
       if (!found_node) {
//added by markos
		if ((pjmesh_use_box_test) && (cur_face->use == -1)) pjmesh_found_useNeg_face = 1;
//end added by markos
        real_node[nreal_node++] = ve ;
       }
      }
     }
    }
    /* an edge with less than two adjacent faces was found */
    edge_with_less_than_two_faces = 1 ;
#if DIA_FIND
    fprintf (stderr, "\nEdge with less than 2 adjacent faces was found!!\n") ;
#endif
   }
  }
  /* if we found an edge with less than two adjacent faces, we should
     reconstruct the polyhedron with the new nodes inserted and check
     again recursively until we don't have any faces like this anymore. */
//commented and added by markos
  //if (edge_with_less_than_two_faces) {
  if ((edge_with_less_than_two_faces) &&
	  ((!pjmesh_use_box_test) || (!pjmesh_found_useNeg_face))) {
//end commented and added by markos
   /* release polyhedron */
   Msh3DPolyReset() ;
   while ( cur_poly = Msh3DPolyNext() ) {
    cur_face = cur_poly->face ;
    Msh3DPolyDelete( cur_face ) ;
   }
   poly_stack = poly_tail = NULL ;
   /* reconstruct the polyhedron for the new nodes inserted. */
   Msh3DBdryReset () ;
   while (cur_face = Msh3DBdryNext ()) {
    use_face = 0 ;
    for (i = 0; i < 3; i++) {
     for (j = 0; j < nreal_node; j++) {
      if (real_node[j] == cur_face->verts[i]) {
       use_face++ ;
       break ;
      }
     }
    }
    if (use_face == 3) {
     face_poly = Msh3DPushPolyFace( cur_face ) ;
    }
   }
//commented by markos
   /* release memory for edges. */
   //Msh3DFree (in_edges) ;
//end commented by markos
  }
  else {
   /* stop checking and return. */
   not_ok = 0 ;
#if DIA_FIND
   fprintf (stderr, "\nNo edge with less than 2 adjacent faces was found!!\n") ;
#endif
  }

//added by markos - it should be here
  /* release memory for edges. */
   Msh3DFree (in_edges) ;
//end added by markos
 }
#endif


/* PJMesh - if the face to be back-tracked is adjacent to a useNeg face (that
		corresponds to an output face, that will be advanced later), then that
		face is recursively adjacent to (perhaps) all the useNeg faces. thus,
		the center point of the polyhedron is likely to be not-visible to
		several faces and, therefore will delete most of the elements created
		by the advancing front algorithm. therefore, this face is also marked
		as a useNeg face, and this back-tracking is canceled. */

	if (pjmesh_use_box_test)
	{
		//double check
		if (pjmesh_found_useNeg_face == 0)
		{
			Msh3DPolyReset();

			while (cur_poly = Msh3DPolyNext ())
			{
				cur_face = cur_poly->face;

				if (cur_face->use == -1)
				{
					pjmesh_found_useNeg_face = 1;

					break;
				}
			}
		}

		if (pjmesh_found_useNeg_face)
		{
			Msh3DPolyReset();

			while (cur_poly = Msh3DPolyNext())
			{
				cur_face = cur_poly->face;

				if (cur_face->use != -1)
				{
					//cur_face->use = -1;
					cur_face->use++;
					if (cur_face->use == 3)
					{
						cur_face->use = -1;
						cur_face->layer = 0;
					}

					if ((cur_face->use == 1) && (bdry_use1 == 0)) bdry_use1 = cur_face;
					else if ((cur_face->use == 2) && (bdry_use2 == 0)) bdry_use2 = cur_face;
					else if ((cur_face->use == -1) && (bdry_useNeg == 0)) bdry_useNeg = cur_face;

	/*//debug markos
					if (((cur_face->verts[0] == 401) && (cur_face->verts[1] == 433) && (cur_face->verts[2] == 578)) ||
						((cur_face->verts[0] == 433) && (cur_face->verts[1] == 401) && (cur_face->verts[2] == 465)) ||
						((cur_face->verts[0] == 465) && (cur_face->verts[1] == 401) && (cur_face->verts[2] == 489)) ||
						((cur_face->verts[0] == 433) && (cur_face->verts[1] == 465) && (cur_face->verts[2] == 578)) ||
						((cur_face->verts[0] == 489) && (cur_face->verts[1] == 401) && (cur_face->verts[2] == 578)) ||
						((cur_face->verts[0] == 489) && (cur_face->verts[1] == 578) && (cur_face->verts[2] == 465)))
					{
						fprintf(stdout, "debug markos, setting face->use = %d for face %d, %d, %d\n", cur_face->use, cur_face->verts[0], cur_face->verts[1], cur_face->verts[2]);
					}
	//end debug markos*/
				}

				Msh3DPolyDelete(cur_face);
			}

			poly_stack = poly_tail = NULL;

			/*while (cur_poly = Msh3DPolyPop())
			{
				cur_face = cur_poly->face;

				if (cur_face->use != -1)
				{
					cur_face->use = -1;
					if (cur_face->use == -1) cur_face->layer = 0;
				}

				Msh3DPolyFree(cur_poly);
			}*/
		}

		//PMsh3DCheckBdryUse();
	}
/* end PJMesh - if the face to be back-tracked is adjacent to a useNeg face (that
		corresponds to an output face, that will be advanced later), then that
		face is recursively adjacent to (perhaps) all the useNeg faces. thus,
		the center point of the polyhedron is likely to be not-visible to
		several faces and, therefore will delete most of the elements created
		by the advancing front algorithm. therefore, this face is also marked
		as a useNeg face, and this back-tracking is canceled. */
}

static void Msh3DFindPoly (
 Msh3DBdryFace       face,
 Msh3DBdryFace       *face_face,
 int                 *face_node,
 int                 *nface_face,
 int                 *nface_node,
 int                 shp,
 int                 pjmesh_use_box_test )
{
 /* find all nodes and faces of the polyhedron */

 *nface_face = *nface_node = 0 ;
 Msh3DFindPolyNodes (face, face_face, face_node, nface_face, nface_node, pjmesh_use_box_test) ;

 /* find the polyhedron */

 Msh3DFindPolyBuild (face, face_node, nface_node, shp, pjmesh_use_box_test) ;
}

static void Msh3DFindPolyOld (
 Msh3DBdryFace       face,
 int                 *face_node,
 int                 nface_node )
{
 Msh3DPolyFace       cur_poly, face_poly ;
 Msh3DAdjFace        cur_face ;
 int                 i, j, ve, found_face, found_node ;

 /* insert given face if it's not already in polyhedron stack */

 found_face = 0 ;
 Msh3DPolyReset () ;
 while (cur_poly = Msh3DPolyNext ()) {
  if (cur_poly->face == face) {
   found_face = 1 ;
   break ;
  }
 }
 if (!found_face) {
  face_poly = Msh3DPushPolyFace( face ) ;
 }

 /* traverse recursivily the face nodes to complete the polyhedron */

 if (!found_face) {
  for (i = 0; i < 3; i++) {
   ve = face->verts[i] ;
   found_node = 0 ;
   for (j = 0; j < nface_node; j++) {
    if (face_node[j] == ve) {
     found_node = 1 ;
     break ;
    }
   }
   if (!found_node) {
    face_node[nface_node++] = ve ;
    for (cur_face = node_list[ve].faces; cur_face; cur_face = cur_face->next)
     Msh3DFindPolyOld (cur_face->face, face_node, nface_node) ;
   }
  }
 }
}

/* -------------------------------------------------------------------
** Msh3DRecoPoly - these routines manage a reconstruct a polyhedron,
**                 given an initial face that belongs to it. It treats
**                 all pathological cases related only to nodes, that
**                 is, two polyhedra linked by only a node are split
**                 in two, to be treated separately.
*/

static void Msh3DRecoPolyNodes (
 Msh3DBdryFace       face,
 Msh3DBdryFace       *face_face,
 int                 *face_node,
 int                 *nface_face,
 int                 *nface_node,
 int                 pjmesh_use_box_test )
{
 Msh3DPolyFace       cur_poly, face_poly ;
 Msh3DAdjFace        cur_face ;
 int                 i, j, ve, found_face, found_node ;

 /* insert given face if it's not already in polyhedron faces */

 found_face = 0 ;
 for (i = 0; i < (*nface_face); i++) {
  if (face_face[i] == face) {
   found_face = 1 ;
   break ;
  }
 }

 /* PJmesh - if face is a remaining, it should be added to polyhedron only
	if all its three vertices belong to the polyhedron */

	if (pjmesh_use_box_test)
	{
		if ((!found_face) && (face->use == -1))
		{
			found_node = 0;

			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < (*nface_node); j++)
				{
					if (face_node[j] == face->verts[i])
					{
						found_node++;
					}
				}
			}

			if (found_node == 3)
			{
				PMsh3DInvertFace(face);
			}
			else
			{
				found_face = 1;
			}
		}
	}

/* end PJmesh - if face is a remaining, it should be added to polyhedron only
	if all its three vertices belong to the polyhedron */

 if (!found_face) {
  face_face[(*nface_face)++] = face ;
 }

 /* traverse recursively the face nodes to complete the polyhedron faces */

 if (!found_face) {
  for (i = 0; i < 3; i++) {
   ve = face->verts[i] ;
   found_node = 0 ;
   for (j = 0; j < (*nface_node); j++) {
    if (face_node[j] == ve) {
     found_node = 1 ;
     break ;
    }
   }
   if (!found_node) {
    face_node[(*nface_node)++] = ve ;
    for (cur_face = node_list[ve].faces; cur_face; cur_face = cur_face->next)
     Msh3DRecoPolyNodes (cur_face->face, face_face, face_node, nface_face, nface_node, pjmesh_use_box_test) ;
   }
  }
 }
}

static void Msh3DRecoPolyBuild (
 Msh3DBdryFace       face,
 int                 *face_node,
 int                 *nface_node,
 int                 shp )
{
 Msh3DPolyFace       cur_poly, face_poly ;
 Msh3DBdryFace       cur_face, real_face[10000] ;
 Msh3DAdjFace        cur, oth, can, nxt ;
 Msh3DBdryFace       fid, nid ;
 int                 i, j, k, l, ve, use_face, found_node, new_node = 1 ;
 int                 m, n, v1, v2, num, p, q ;
 int                 real_node[10000], nreal_node = 0, nreal_face = 0 ;

 /* insert given face nodes in real_nodes vector */

 for (i = 0; i < 3; i++) {
  ve = face->verts[i] ;
  found_node = 0 ;
  for (j = 0; j < nreal_node; j++) {
   if (real_node[j] == ve) {
    found_node = 1 ;
    break ;
   }
  }
  if (!found_node) {
   real_node[nreal_node++] = ve ;
  }
 }

 /* introduce in real_nodes vector only the face_nodes that have at least
    one face with two nodes in real_nodes, which represents that this node
    is in the same side of real_nodes introduced before, to avoid cases of
    non-manifold polyhedra. This process stops when no new node is left in
    face_nodes vector to be introduced in real_nodes vector. */

 while( new_node ) {

  /* initiate flag for new_node introduced */

  new_node = 0 ;

  /* build the polyhedron based in real_nodes vector */

  for (i = 0; i < (*nface_node); i++) {

   /* look at all adjacent faces of the node */

   for (cur = node_list[face_node[i]].faces; cur; cur = cur->next) {

    /* verify if the face has at least two real_nodes different of current
       node, which represents that this node should be introduced in
       real_nodes vector, if it wasn't before. */

    found_node = 0 ;
    for( j = 0; j < 3; j++ ) {
     if( cur->face->verts[j] == face_node[i] ) continue ;
     for( k = 0; k < (*nface_node); k++) {
      if( cur->face->verts[j] == real_node[k] ) {
       found_node++ ;
       break ;
      }
     }
    }
    if( found_node == 2 ) {
     for (k = 0; k < 3; k++) {
      ve = cur->face->verts[k] ;
      found_node = 0 ;
      for (j = 0; j < nreal_node; j++) {
       if (real_node[j] == ve) {
        found_node = 1 ;
        break ;
       }
      }
      if (!found_node) {
       real_node[nreal_node++] = ve ;
       new_node = 1 ;
      }
     }
    }

   }
  }
 }

 /* now build the polyhedron stack based in real polyhedron nodes. For a
    face that is in boundary stack be introduced in polyhedron stack, it
    should have all three nodes as real nodes */

 Msh3DBdryReset () ;
 while (cur_face = Msh3DBdryNext ()) {
  use_face = 0 ;
  for (i = 0; i < 3; i++) {
   for (j = 0; j < nreal_node; j++) {
    if (real_node[j] == cur_face->verts[i]) {
     use_face++ ;
     break ;
    }
   }
  }
  if (use_face == 3) {
   face_poly = Msh3DPushPolyFace( cur_face ) ;
  }
 }
}

static void Msh3DRecoPoly (
 Msh3DBdryFace       face,
 Msh3DBdryFace       *face_face,
 int                 *face_node,
 int                 *nface_face,
 int                 *nface_node,
 int                 shp,
 int                 pjmesh_use_box_test )
{
 /* find all nodes and faces of the polyhedron */

 *nface_face = *nface_node = 0 ;
 Msh3DRecoPolyNodes (face, face_face, face_node, nface_face, nface_node, pjmesh_use_box_test) ;

 /* find the polyhedron */

 Msh3DRecoPolyBuild (face, face_node, nface_node, shp) ;
}


static int Msh3DEqualFaces (int face1[3], int face2[3])
{
 int                i, j ;
 int                num = 0 ;

 /* check if given faces are equal */

 for (i = 0; i < 3; i++) {
  for (j = 0; j < 3; j++) {
   if (face1[i] == face2[j])
    num++ ;
  }
 }

 /* if num is equal to 3 is because they are equal and then return 1 */

 if (num == 3)  return 1 ;
 else           return 0 ;
}

static int Msh3DBoundaryFace (Msh3DBdryFace face, int verts[3])
{
 Msh3DAdjIniFace    cur ;
 int                i, k ;
 int                check = 0 ;

 /* check if given face belongs to the initial boundary */

#if OLD_BDRY
 for (i = 0; i < nbdrynode; i++)
 {
  if (node_list[i].ifaces)
  {
   for (cur = node_list[i].ifaces; cur; cur = cur->next)
   {
    check = 0 ;
    for (k = 0; k < 3; k++)
    {
     if ( (verts[0] == cur->verts[k]) ||
          (verts[1] == cur->verts[k]) ||
          (verts[2] == cur->verts[k]) )
      check++ ;
    }
    if (check == 3) break ;
   }
  }
  if (check == 3) break ;
 }
#else
 if (face->layer == 0) check = 3 ;
 else                  check = 0 ;
#endif

//added by markos
 //if (face->use == -1) check = 3;
//end added by markos

 /* return check value. It will be 1 if given face belongs to the
    initial boundary and zero if not. */

 if (check == 3)  return 1 ;
 else             return 0 ;
}

static int Msh3DNoElemFace (
 Msh3DBdryFace     face,
 int               verts[3],
 int               *elem,
 int               *nelem )
{
 int           i, k, ind, elm, check = 0 ;

 /* check if given face has an element associated with it. */

#if OLD_ELEM
 for ( i = 0 ; i < *nelem; i++ ) {
  ind = i*4 ;
  elm = 0 ;
  for ( k = 0; k < 3 ; k++ )
  {
   if ( (verts[k]==elem[ind+0]) || (verts[k]==elem[ind+1]) ||
        (verts[k]==elem[ind+2]) || (verts[k]==elem[ind+3]) )
    elm++ ;
  }
  if( elm == 3 ) {
   check = 1 ;
   break ;
  }
 }
#else
 if ((face->rgn[0] == -1) && (face->rgn[1] == -1)) check = 0 ;
 else                                              check = 1 ;
#endif

 /* return check value. It will be 1 if given face doesn't have any element
    associated with it and 0 if it has. */

 if (!check)      return 1 ;
 else             return 0 ;
}

static int Msh3DCreationFace (
 int               verts[3],
 int               *elem,
 int               *nelem,
 int               pjmesh_use_box_test )
{
 int                i, j, m, n, k, ind, elm, fac, check = 0 ;
 int                v0, v1, v2, f_verts[3] ;
 Msh3DAdjIniFace    cur ;
 Msh3DPolyFace      cur_poly ;
 Msh3DBdryFace      face ;

 /* check if new face to be created belongs to the original boundary */

  for ( i = 0 ; i < *nelem; i++ ) {
   /* get the element */
   ind = i*4 ;
   /* verify if face belongs to the element */
   elm = 0 ;
   for ( k = 0; k < 3 ; k++ )
   {
    if ( (verts[k]==elem[ind+0]) || (verts[k]==elem[ind+1]) ||
	 (verts[k]==elem[ind+2]) || (verts[k]==elem[ind+3]) )
     elm++ ;
   }
   /* given face belongs to the element */
   if ( elm == 3 )
   {
    /* get element faces */
    for ( j = 0; j < 4; j++ )
    {
     if ( j == 0 ) v0 = elem[ind+1], v1 = elem[ind+3], v2 = elem[ind+2] ;
     if ( j == 1 ) v0 = elem[ind+2], v1 = elem[ind+3], v2 = elem[ind+0] ;
     if ( j == 2 ) v0 = elem[ind+3], v1 = elem[ind+1], v2 = elem[ind+0] ;
     if ( j == 3 ) v0 = elem[ind+0], v1 = elem[ind+1], v2 = elem[ind+2] ;
     /* verify if the element face is different from the input face */
     fac = 0 ;
     for ( k = 0 ; k < 3 ; k++ ) {
      if ( (verts[k] == v0) || (verts[k] == v1) || (verts[k] == v2) )
       fac++ ;
     }
     /* if the face is different, it means that it is a new face */
     if ( fac != 3 ) {
      /* check if the new face is already in the polyhedron */
      Msh3DPolyReset() ;
      while (cur_poly = Msh3DPolyNext() ) {
       fac = 0 ;
       face = cur_poly->face ;
       for ( k = 0 ; k < 3; k++ ) {
        if ( (face->verts[k] == v0) ||
             (face->verts[k] == v1) ||
             (face->verts[k] == v2) )
         fac++ ;
       }
       if (fac == 3) break ;
      }
      /* if the new face is not in the polyhedron do more checks */
      if (fac != 3) {
       /* check if new face belongs to the original boundary */
       f_verts[0] = v0 ;
       f_verts[1] = v1 ;
       f_verts[2] = v2 ;
       for (m = 0; m < nbdrynode; m++) {
        if (node_list[m].ifaces) {
         for (cur = node_list[m].ifaces; cur; cur = cur->next) {
          check = 0 ;
	  for (n = 0; n < 3; n++) {
           if ( (f_verts[0] == cur->verts[n]) ||
	        (f_verts[1] == cur->verts[n]) ||
                (f_verts[2] == cur->verts[n]) )
            check++ ;
	  }
	  if (check == 3) break ;
	 }
	}
	if (check == 3) break ;
       }

/* PJMesh - check if the tetrahedron has a face on the remaining front*/
		if (pjmesh_use_box_test)
		{
		   if ((check != 3) && (PMsh3DIsRemainingFrontFace(v0, v1, v2)))
		   {
				check = 3;
		   }
		}
/* end PJMesh - check if the tetrahedron has a face on the remaining front*/

       if (check == 3)
	return 1 ;  /* face to be created is on the original boundary */
      }
     }
    }
   }
  }

 /* if we get here, it means that no new faces created belongs to the
  * original boundary. */

 return 0 ;
}

static int Msh3DDelRepeatedFace (int pjmesh_use_box_test)
{
  int            recon, found, face_num, v[3], w[3], i, j, k ;
  int            nface_node = 0 ;
  int            nface_face = 0 ;
  int            *face_node ;
  Msh3DBdryFace  *face_face ;
  Msh3DPolyFace  new_poly, cur_poly, cur_next ;
  Msh3DBdryFace  face, next, new_bdry ;
  Msh3DBdryFace  face_dele[1000] ;
  Msh3DAdjFace   cur, *temp, save ;

  /* Get memory for face and nodes */

  face_node = (int *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(int) ) ;
  face_face = (Msh3DBdryFace *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(Msh3DBdryFace) ) ;

  /* Delete repeated face from polyhedron and reconstruct it. This
   * only happens when a current polyhedron reaches another one and
   * the face being deleted is already in the front. */

  recon = 0 ;
  face_num = 0 ;
  /* first verify if we have repeated face */
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   for ( i = 0; i < 3; i++ )
    v[i] = face->verts[i] ;
   cur_next = cur_poly->next ;
   while (cur_next) {
    next = cur_next->face ;
    for ( i = 0; i < 3; i++ )
     w[i] = next->verts[i] ;
    if( Msh3DEqualFaces (v, w) ) {
     /* if we get here we have two identical faces
      * on the polyhedron. Mark them to delete after. */
     if (!Msh3DBoundaryFace (face, v)) {
      found = 0 ;
      for (i = 0; i < face_num; i++) {
       if (face_dele[i] == face) {
        found = 1 ;
        break ;
       }
      }
      if (!found) {
       face_dele[face_num] = face ;
       face_num++ ;
      }
     }
     if (!Msh3DBoundaryFace (next, w)) {
      found = 0 ;
      for (i = 0; i < face_num; i++) {
       if (face_dele[i] == next) {
        found = 1 ;
        break ;
       }
      }
      if (!found) {
       face_dele[face_num] = next ;
       face_num++ ;
      }
     }
     recon = 1 ;
    }
    cur_next = cur_next->next ;
   }
  }
  /* if we have repeated face, then delete */
  if (recon) {
   /* get face that is not going to be deleted to reconstruct polyhedron */
   Msh3DPolyReset() ;
   while ( cur_poly = Msh3DPolyNext() ) {
    face = cur_poly->face ;
    found = 0 ;
    for (i = 0; i < face_num; i++) {
     if (face_dele[i] == face) {
      found = 1 ;
      break ;
     }
    }
    if (!found) {
     new_bdry = face ;
     break ;
    }
   }
   /* delete repeated faces */
   for (i = 0; i < face_num; i++) {
    face = face_dele[i] ;
    for ( k=0 ; k<3 ; k++ ) {
     for ( temp = &(node_list[face->verts[k]].faces) ; *temp ;
           temp = &((*temp)->next) ) {
      if ( (*temp)->face == face ) {
       save = *temp ;
       *temp = (*temp)->next ;
       Msh3DAdjFree( save ) ;
       break ;
      }
     }
     if ( !node_list[face->verts[k]].faces )
      node_list[face->verts[k]].active_flag = 0 ;
    }
    Msh3DPolyDelete( face ) ;
    Msh3DBdryDelete( face ) ;
   }
  }
  /* reconstruct the polyhedron if faces were deleted using reconstruct
   * polyhedron because we don't need to deal with pathological cases in
   * edges and nodes shared by two or more polyhedra. */
  if (recon) {
   Msh3DPolyReset() ;
   while ( cur_poly = Msh3DPolyNext() ) {
    face = cur_poly->face ;
    Msh3DPolyDelete( face ) ;
   }
   poly_stack = poly_tail = NULL ;
   Msh3DRecoPoly (new_bdry, face_face, face_node, &nface_face, &nface_node, 1, pjmesh_use_box_test) ;
  }

  /* free allocated memory */

  Msh3DFree( face_node ) ;
  Msh3DFree( face_face ) ;
}

static int Msh3DVerifFace (int verts[3], Msh3DBdryFace *face)
{
 Msh3DBdryFace      cur ;
 int                k, fac ;

 /* check if given face belongs to the boundary */

 Msh3DBdryReset() ;
 while (cur = Msh3DBdryNext() ) {
  fac = 0 ;
  for ( k = 0 ; k < 3; k++ ) {
   if ( (cur->verts[k] == verts[0]) ||
        (cur->verts[k] == verts[1]) ||
        (cur->verts[k] == verts[2]) )
    fac++ ;
  }
  if (fac == 3) *face = cur ;
  if (fac == 3) break ;
 }

 /* return check value. It will be 1 if given face belongs to the
    boundary and zero if not. */

 if (fac == 3) return 1 ;
 else          return 0 ;
}

static Msh3DBdryFace Msh3DFindFace (
 Msh3DBdryFace     face,
 int		   fid,
 int		   sid )
{
 Msh3DAdjFaceList face1, aux1 ;
 Msh3DAdjFaceList face2, aux2 ;
 Msh3DBdryFace    facef = NULL ;
 int              found = 0 ;
 int              i, tid ;
 double           cand_vec[3], dot ;

 /* find face adjacent of given face by given edge. This face is only
    considered if its normal it's in the same semi-plane of the given
    face */

 face1 = node_list[fid].faces;
 face2 = node_list[sid].faces;
 aux1 = face1;
 while (aux1)
 {
  aux2 = face2;
  while (aux2)
  {
   if((aux1->face==aux2->face) && (aux2->face!=face))
   {
    found = 1;
    break;
   }
   else
    aux2 = aux2->next;
  }
  if (found)
  {
   facef = aux2->face;
   break;
  }
  else
   aux1 = aux1->next;
 }

 if (facef == NULL)
  return facef;

 for (i = 0; i < 3; i++)
  if (facef->verts[i] != fid && facef->verts[i] != sid)
   tid = facef->verts[i] ;

 cand_vec[0] = node_list[tid].coord[0] - face->center[0] ;
 cand_vec[1] = node_list[tid].coord[1] - face->center[1] ;
 cand_vec[2] = node_list[tid].coord[2] - face->center[2] ;

 dot = cand_vec[0]*face->r_x_s[0] +
       cand_vec[1]*face->r_x_s[1] +
       cand_vec[2]*face->r_x_s[2] ;

 if ( dot < 0.0 ) return NULL ;
 else             return facef ;
}

static void Msh3DFindNode (
 Msh3DBdryFace       face,
 int                 debug,
 double		     node[3] )
{
 int		   j ;
 int		   num_node ;
 double            l ;
 double		   h ;
 double            d ;
 double            nrm[3] ;

 /* get the triangle equilateral size of the same area of the face */

 l = sqrt( (4*face->area) / sqrt(3.0) );

 /* evaluate the height to have all faces of a possible new element more
    equal of an equilateral triangle than possible */

 h = (l * sqrt(13.0)) / 4.0;

 /* normalize the face normal */

 d = sqrt( face->r_x_s[0] * face->r_x_s[0] +
	   face->r_x_s[1] * face->r_x_s[1] +
	   face->r_x_s[2] * face->r_x_s[2] );
 if (d > 0.0) {
  nrm[0] = face->r_x_s[0] / d;
  nrm[1] = face->r_x_s[1] / d;
  nrm[2] = face->r_x_s[2] / d;
 }
 else {
  nrm[0] = 0.0 ;
  nrm[1] = 0.0 ;
  nrm[2] = 0.0 ;
 }

 /* evaluate the new node coordinate */

 node[0] = face->center[0] + (h/debug)*nrm[0];
 node[1] = face->center[1] + (h/debug)*nrm[1];
 node[2] = face->center[2] + (h/debug)*nrm[2];
}

static void Msh3DTreeNode (
 Msh3DBdryFace       face,
 int                 debug,
 double		     node[3] )
{
 int		   num_node, level ;
 double		   h ;
 double            d ;
 double            nrm[3], new[3] ;

 /* get the size of the cell where the center of the face is */

 h = Msh3DOptimalNodes ( face->center, new, &level ) ;

 /* normalize the face normal */

 d = sqrt( face->r_x_s[0] * face->r_x_s[0] +
	   face->r_x_s[1] * face->r_x_s[1] +
	   face->r_x_s[2] * face->r_x_s[2] );
 nrm[0] = face->r_x_s[0] / d;
 nrm[1] = face->r_x_s[1] / d;
 nrm[2] = face->r_x_s[2] / d;

 /* evaluate the new node coordinate */

 node[0] = face->center[0] + (h/debug)*nrm[0];
 node[1] = face->center[1] + (h/debug)*nrm[1];
 node[2] = face->center[2] + (h/debug)*nrm[2];
}

static void  Msh3DPushNode (
 Msh3DId  *aux,
 double	  *nod,
 int	  *fac,
 int	  *num_nod,
 int       num_fac,
 int       id,
 int       l )
{
 int       i ;
 int 	   j ;
 int	   found ;

 /* search this node in the vector of nodes */

 found = 0;
 for (i = 0; i < *num_nod; i++) {
  if (aux[i].id == id)
  {
   found = 1;
   break;
  }
 }

 /* insert or return the id */

 if (!found)
 {
  for (j = 0; j < 3; j++)
   nod[(*num_nod)*3+j]  = node_list[id].coord[j];
  aux[*num_nod].id  = id;
  aux[*num_nod].pos = *num_nod;
  fac[num_fac*3+l]  = *num_nod;
  (*num_nod)++;
 }
 else
 {
  fac[num_fac*3+l] = aux[i].pos;
 }
}

/* -------------------------------------------------------------------
** Msh3DCheckPoly -  this routine makes geometrical checks to make sure
**                   that no faces of a candidate element cross the
**                   the current polyhedron.
*/

static int  Msh3DCheckPoly(
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                debug,
    int                print,
    int               *elem,
    int               *nelem,
    Msh3DBdryFace     *visi,
    int                dsp )
{
    int    i, j, k, valid, check ;
    double min[3], max[3] ;
    int    v1, v2, type, ind, use, on_bdry ;
    Msh3DPolyFace  poly ;
    Msh3DPolyFace  curr ;
    Msh3DBdryFace  unvi ;
    Msh3DBdryFace  face ;
    Msh3DBdryFace  current ;
    Msh3DBdryFaceRec new ;
    double tol ;
    int    p[3], q[3], f[3] ;
    double a[3], b[3], c[3], d[3], n[3], u[3], v[3], w[3], dot ;

    /* initiate intersection and visualization parameters */

    check = 1 ;
    use   = 0 ;
    unvi  = 0 ;

    /* initiate use of all face as zero in the polyhedron */

#if OLD_VISI
    Msh3DPolyReset () ;
    while (curr = Msh3DPolyNext ()) {
     current = curr->face ;
     current->use = 0 ;
    }
#else
    if ((*visi) == 0) {
     Msh3DPolyReset () ;
     while (curr = Msh3DPolyNext ()) {
      current = curr->face ;
      current->use = 0 ;
     }
    }
#endif

    /* traverse all faces to check if node_indx is valid for polyhedron */

    for (poly = poly_stack; poly; poly = poly->next) {

     /* get the face */

     face = poly->face ;

     /* see if dot product formed with node_indx and face is positive and
        return if an unvisible face was already found in previous step */

     for (i = 0; i < 3; i++) {
      d[i] = node_list[node_indx].coord[i] - face->center[i] ;
      n[i] = face->r_x_s[i] ;
     }
     dot = d[0]*n[0] + d[1]*n[1] + d[2]*n[2] ;
     if (dot <= 0.0) {
      check = 0 ;
      if ( (*visi) != 0 )
       return 0 ;
     }

     /* get the max and min coords for the proposed element */

     for ( i=0 ; i<3 ; i++ ) {
         min[i] = face->min[i] ;
         max[i] = face->max[i] ;
         if ( node_list[node_indx].coord[i] < min[i] )
             min[i] = node_list[node_indx].coord[i] ;
         if ( node_list[node_indx].coord[i] > max[i] )
             max[i] = node_list[node_indx].coord[i] ;
     }

     /* loop through all the faces in the current boundary and see
        if they intersect any of the faces of the proposed new element */

#if DSP_MESH
     if (FDEBUG && DebugFail ) {
       if (dsp) {
        GdbSetTopOff (GDB_DELFACE);
        GdbSetTopOff (GDB_NEWFACE);
        GdbSetTopOff (GDB_VERTEX);
        GdbSetTopOff (GDB_EDGE);
        GdbClear();
       }
     }
#endif

     valid = 1 ;
     Msh3DPolyReset() ;

     while ( valid && (curr = Msh3DPolyNext()) ) {

        /* get current face */

        current = curr->face ;

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* test if the current face is the same of given face, because
           if so no intersection test is necessary. */

        if ( current == face )
            continue ;

        /* test the intersection between the boundary triangle we are
           currently checking with the three new faces. */

        for ( j = 0 ; j < 3 ; j++ ) {
         a[j] = node_list[current->verts[0]].coord[j] ;
         b[j] = node_list[current->verts[1]].coord[j] ;
         c[j] = node_list[current->verts[2]].coord[j] ;
         f[j] = current->verts[j] ;
         p[j] = 0 ;
        }

        for ( k = 0 ; k < 3 ; k++ ) {

         for ( j = 0 ; j < 3 ; j++ ) {
          u[j] = node_list[face->verts[k]].coord[j] ;
          v[j] = node_list[face->verts[(k+1)%3]].coord[j] ;
          w[j] = node_list[node_indx].coord[j] ;
          q[j] = 0 ;
         }

         type = Msh3DTmp7 ( 1, a, b, c, p, u, v, w, q ) ;

#if OLD_VISI
         if( type == 0 ) {
          current->use++ ;
          check = 0 ;
          if ((*visi) != 0) {
           return 0 ;
          }
          else {
           bdry  = Msh3DBoundaryFace (current, f) ;
           if( bdry == 0 ) {
            if( current->use > use ) {
             unvi = current ;
             use  = current->use ;
            }
           }
          }
         }
#else
         if( type == 0 ) {
          check = 0 ;
          if ((*visi) != 0) {
           return 0 ;
          }
          else {
           current->use++ ;
           on_bdry = Msh3DBoundaryFace (current, f) ;
           if( (on_bdry == 0) ) {
            if( current->use > use ) {
             unvi = current ;
             use  = current->use ;
            }
           }
          }
         }
#endif

         if( type == 0 ) break ;

        }

     }
    }


    /* check most unvisible face */

    if (!check) {
     if ((*visi) == 0) *visi = unvi ;
     return 0 ;
    }

    /* if we get here, the new element is valid */

#if DSP_PRNT
    if ( valid && print )
     fprintf(stderr,"Valid  insertion of new node\n" ) ;
#endif

    return(valid) ;
}

/* -------------------------------------------------------------------
** Msh3DCheckTrans - this routine makes transition checks to make
**                   sure that the biggest new face (to be formed)
**                   is no bigger than ten times the biggest face
**                   adjacent to the candidate node.
*/

static int  Msh3DCheckTrans(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                node_bdry,
    int                debug,
    int                print,
    int               *elem,
    int               *nelem )
{
    int    i, v1i, v2i ;
    double r[3], s[3], r_x_s[3], area ;
    double max_new_area, max_adj_area ;
    int    v1, v2, v3 ;
    Msh3DAdjFace   cur ;

#if OLD_TRAN
    /* Don't check transition for any phase */

    return 1 ;
#else
    /* you don't want to do that in the topology-phase.
     * Also you don't want to do this if the node_indx
     * is on the original boundary, because there is
     * nothing we can do and we have to accept the node */

    if (face->use > 0) return 1 ;
    /*if (node_indx < nbdrynode) return 1 ;*/

    /* first compute the area of the biggest face to be formed */

    max_new_area = 0 ;
    for ( v1i=1,v2i=0 ; v2i<3 ; v1i=((v1i+1)%3), v2i++ ) {
	v1 = face->verts[v1i] ;
	v2 = face->verts[v2i] ;
	v3 = node_indx ;
	for ( i=0 ; i<3 ; i++ ) {
	    r[i] = node_list[v2].coord[i] - node_list[v1].coord[i] ;
	    s[i] = node_list[v3].coord[i] - node_list[v1].coord[i] ;
        }
	r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
	r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
	r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
	area = sqrt( r_x_s[0] * r_x_s[0] +
		     r_x_s[1] * r_x_s[1] +
		     r_x_s[2] * r_x_s[2] ) / 2.0 ;
        if (area > max_new_area) max_new_area = area ;
    }

    /* now compute the biggest are for all faces adjacent to the
     * candidate node, in order to compare with the biggest new face */

    max_adj_area = 0 ;
    for ( cur=node_list[node_indx].faces ; cur ; cur=cur->next ) {
     area = cur->face->area ;
     if (area > max_adj_area) max_adj_area = area ;
    }

    /* now compare the areas. If the biggest new are is ten times
     * bigger than the biggest adjacent area, it means that a big
     * base face is picking a node adjacent to much smaller faces
     * to form new elements, and this is not good for transition */

    if (max_new_area > (5*max_adj_area)) {
     if (print) {
      printf ("Transition problem:\n") ;
      printf ("Face=%d\t%d\t%d\tNode=%d\n", face->verts[0],
		                            face->verts[1],
		                            face->verts[2],
		                            node_indx) ;
      if (node_indx < nbdrynode)
       printf ("Node candidate on the original boundary\n") ;
     }
     return 0 ;
    }
    else {
     return 1 ;
    }
#endif
}

/* -------------------------------------------------------------------
** Msh3DCheckValid - this routine makes geometrical checks to make sure
**                   that no faces of a candidate element cross the
**                   the current boundary.
*/

#if OLD_INTE
static int  Msh3DCheckValid(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                node_bdry,
    int                debug,
    int                print,
    int               *elem,
    int               *nelem )
{
    int    i, j, k, l, m, n, valid, inter ;
    double min[3], max[3] ;
    int    v1, v2, type, ind ;
    Msh3DBdryFace  current ;
    Msh3DBdryFace  inte ;
    Msh3DBdryFaceRec new ;
    double tol ;
    int    p[3], q[3] ;
    double a[3], b[3], c[3], u[3], v[3], w[3] ;
#if AD_TREE
    int    *range_result ;
    int    range_size, front_size ;
    int    node_id[3] ;
    RangeCoord minc, maxc ;
    double     minf[3], maxf[3] ;
#endif
    int        curol, curfa, found ;
    double     l1, l2, l3 ;
    int        all_faces = 0, big_faces = 1 ;

    /* get the max and min coords for the proposed element */

    for ( i=0 ; i<3 ; i++ ) {
        min[i] = face->min[i] ;
        max[i] = face->max[i] ;
        if ( node_list[node_indx].coord[i] < min[i] )
            min[i] = node_list[node_indx].coord[i] ;
        if ( node_list[node_indx].coord[i] > max[i] )
            max[i] = node_list[node_indx].coord[i] ;
    }

    /* loop through all the faces in the current boundary and see
       if they intersect any of the faces of the proposed new element */

#if DSP_MESH
    if (FDEBUG && DebugFail ) {
       GdbSetTopOff (GDB_DELFACE);
       GdbSetTopOff (GDB_NEWFACE);
       GdbSetTopOff (GDB_VERTEX);
       GdbSetTopOff (GDB_EDGE);
       GdbClear();
    }
#endif

#if AD_TREE
    /* Check the intersection only against faces in the AD_TREE */

    /* Find max and min */
    minc[0] = face->min[0] ;
    minc[1] = face->min[1] ;
    minc[2] = face->min[2] ;
    maxc[0] = face->max[0] ;
    maxc[1] = face->max[1] ;
    maxc[2] = face->max[2] ;
    for (n=1 ; n<4 ; n++ ) {
     node_id[0] = face->verts[n-1] ;
     node_id[1] = face->verts[n%3] ;
     node_id[2] = node_indx ;
     for (l=0; l < 3; l++) {
      maxf[l] = node_list[node_id[0]].coord[l] ;
      minf[l] = node_list[node_id[0]].coord[l] ;
      if (maxf[l] < node_list[node_id[1]].coord[l])
       maxf[l] = node_list[node_id[1]].coord[l] ;
      if (minf[l] > node_list[node_id[1]].coord[l])
       minf[l] = node_list[node_id[1]].coord[l] ;
      if (maxf[l] < node_list[node_id[2]].coord[l])
       maxf[l] = node_list[node_id[2]].coord[l] ;
      if (minf[l] > node_list[node_id[2]].coord[l])
       minf[l] = node_list[node_id[2]].coord[l] ;
     }
     if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
     if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
     if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
     if (minc[0] > minf[0]) minc[0] = minf[0] ;
     if (minc[1] > minf[1]) minc[1] = minf[1] ;
     if (minc[2] > minf[2]) minc[2] = minf[2] ;
    }

#if AD_DEBU
    /* check the comparison between the number of faces that would
     * be checked with and without the use of the AD_TREE */
    curol = 0 ;
    Msh3DBdryReset() ;
    while ( current = Msh3DBdryNext() ) curol++ ;
    range_result = QueryRangeTree(minc,maxc,&range_size) ;
    curfa = range_size ;
    if (all_faces) {
     printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                face->verts[1],
 				                face->verts[2]) ;
     if (face->use == 0)
      printf("Geometry: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 1)
      printf("Topology: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 2)
      printf("Tracking: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     printf ("\n") ;
    }
    if (big_faces) {
     if (curfa > curol) {
      printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                 face->verts[1],
  				                 face->verts[2]) ;
      printf("Normal=%d Ad_Tree=%d\n",curol,curfa) ;
      printf ("\n") ;
     }
    }
    QueryCleanRangeTree ( ) ;
#endif

    /* Use AD Tree to speed up the intersection checking. Check if
     * number of faces from search is bigger than all faces currently
     * on the front, which means that the AD_TREE is unbalaced. If so,
     * abandon the AD_TREE and check against faces on the front. */
    valid = 1 ;
    range_result = QueryRangeTree(minc,maxc,&range_size) ;
    for (m=0; m < range_size; m++) {

	/* get the tolerance and the face to be checked */
        current = adtreehash[range_result[m]] ;
	if (current == 0) continue ;
        tol = current->area * tol_factor ;

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* test if the current face is the same of given face, because
           if so no intersection test is necessary. */

        if ( current == face )
            continue ;

        /* test the intersection between the boundary triangle we are
           currently checking with the three new faces. */

        for ( j = 0 ; j < 3 ; j++ ) {
         a[j] = node_list[current->verts[0]].coord[j] ;
         b[j] = node_list[current->verts[1]].coord[j] ;
         c[j] = node_list[current->verts[2]].coord[j] ;
         p[j] = 0 ;
        }

        for ( k = 0 ; k < 3 ; k++ ) {

         for ( j = 0 ; j < 3 ; j++ ) {
          u[j] = node_list[face->verts[k]].coord[j] ;
          v[j] = node_list[face->verts[(k+1)%3]].coord[j] ;
          w[j] = node_list[node_indx].coord[j] ;
          q[j] = 0 ;
         }

         type = Msh3DTmp7 ( 1, a, b, c, p, u, v, w, q ) ;

         if( type == 0 ) {
#if DSP_MESH
            if ( FDEBUG && DebugFail )
              GdbDspFail (node_indx,v1,v2);
#endif
#if AD_DEBU
	    /* check to see if intersected face is in AD_TREE */
	    found = 0 ;
            for (m=0; m < range_size; m++) {
             inte = adtreehash[range_result[m]] ;
	     if (inte == current) {
	      found = 1 ;
	      break ;
             }
            }
	    if (!found) {
             fprintf (stderr,"Face=%d %d %d for Face=%d %d %d not in AD_TREE\n",
		     current->verts[0], current->verts[1], current->verts[2],
		     face->verts[0],    face->verts[1],    face->verts[2]) ;
            }
#endif
            QueryCleanRangeTree ( ) ;
            return 0;
         }
        }

        /* test to see if new point is near from current face. If it's
           then this point should not be considered */

        type = Msh3DTmp3( current, face, node_list, node_indx, debug );

	if( type == 0 ) {
         QueryCleanRangeTree ( ) ;
	 return 0;
        }
    }
    QueryCleanRangeTree ( ) ;

#else

    /* Check intersection against all faces on the current boundary */
    valid = 1 ;

#if AD_DEBU
    /* check the comparison between the number of faces that would
     * be checked with and without the use of the AD_TREE */
    minc[0] = face->min[0] ;
    minc[1] = face->min[1] ;
    minc[2] = face->min[2] ;
    maxc[0] = face->max[0] ;
    maxc[1] = face->max[1] ;
    maxc[2] = face->max[2] ;
    for (n=1 ; n<4 ; n++ ) {
     node_id[0] = face->verts[n-1] ;
     node_id[1] = face->verts[n%3] ;
     node_id[2] = node_indx ;
     for (l=0; l < 3; l++) {
      maxf[l] = node_list[node_id[0]].coord[l] ;
      minf[l] = node_list[node_id[0]].coord[l] ;
      if (maxf[l] < node_list[node_id[1]].coord[l])
       maxf[l] = node_list[node_id[1]].coord[l] ;
      if (minf[l] > node_list[node_id[1]].coord[l])
       minf[l] = node_list[node_id[1]].coord[l] ;
      if (maxf[l] < node_list[node_id[2]].coord[l])
       maxf[l] = node_list[node_id[2]].coord[l] ;
      if (minf[l] > node_list[node_id[2]].coord[l])
       minf[l] = node_list[node_id[2]].coord[l] ;
     }
     if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
     if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
     if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
     if (minc[0] > minf[0]) minc[0] = minf[0] ;
     if (minc[1] > minf[1]) minc[1] = minf[1] ;
     if (minc[2] > minf[2]) minc[2] = minf[2] ;
    }
    curol = 0 ;
    Msh3DBdryReset() ;
    while ( current = Msh3DBdryNext() ) curol++ ;
    range_result = QueryRangeTree(minc,maxc,&range_size) ;
    curfa = range_size ;
    if (all_faces) {
     printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                face->verts[1],
 				                face->verts[2]) ;
     if (face->use == 0)
      printf("Geometry: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 1)
      printf("Topology: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 2)
      printf("Tracking: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     printf ("\n") ;
    }
    if (big_faces) {
     if (curfa > curol) {
      printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                 face->verts[1],
  				                 face->verts[2]) ;
      printf("Normal=%d Ad_Tree=%d\n",curol,curfa) ;
      printf ("\n") ;
     }
    }
    QueryCleanRangeTree ( ) ;
#endif

    Msh3DBdryReset() ;
    while ( valid && (current = Msh3DBdryNext()) ) {

	/* get the tolerance */
        tol = current->area * tol_factor ;

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* test if the current face is the same of given face, because
           if so no intersection test is necessary. */

        if ( current == face )
            continue ;

        /* test the intersection between the boundary triangle we are
           currently checking with the three new faces. */

        for ( j = 0 ; j < 3 ; j++ ) {
         a[j] = node_list[current->verts[0]].coord[j] ;
         b[j] = node_list[current->verts[1]].coord[j] ;
         c[j] = node_list[current->verts[2]].coord[j] ;
         p[j] = 0 ;
        }

        for ( k = 0 ; k < 3 ; k++ ) {

         for ( j = 0 ; j < 3 ; j++ ) {
          u[j] = node_list[face->verts[k]].coord[j] ;
          v[j] = node_list[face->verts[(k+1)%3]].coord[j] ;
          w[j] = node_list[node_indx].coord[j] ;
          q[j] = 0 ;
         }

         type = Msh3DTmp7 ( 1, a, b, c, p, u, v, w, q ) ;

         if( type == 0 ) {
#if DSP_MESH
            if ( FDEBUG && DebugFail )
              GdbDspFail (node_indx,v1,v2);
#endif
#if AD_DEBU
	    /* check to see if intersected face is in AD_TREE */
            minc[0] = face->min[0] ;
            minc[1] = face->min[1] ;
            minc[2] = face->min[2] ;
            maxc[0] = face->max[0] ;
            maxc[1] = face->max[1] ;
            maxc[2] = face->max[2] ;
            for (n=1 ; n<4 ; n++ ) {
             node_id[0] = face->verts[n-1] ;
             node_id[1] = face->verts[n%3] ;
             node_id[2] = node_indx ;
             for (l=0; l < 3; l++) {
              maxf[l] = node_list[node_id[0]].coord[l] ;
              minf[l] = node_list[node_id[0]].coord[l] ;
              if (maxf[l] < node_list[node_id[1]].coord[l])
               maxf[l] = node_list[node_id[1]].coord[l] ;
              if (minf[l] > node_list[node_id[1]].coord[l])
               minf[l] = node_list[node_id[1]].coord[l] ;
              if (maxf[l] < node_list[node_id[2]].coord[l])
               maxf[l] = node_list[node_id[2]].coord[l] ;
              if (minf[l] > node_list[node_id[2]].coord[l])
               minf[l] = node_list[node_id[2]].coord[l] ;
             }
             if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
             if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
             if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
             if (minc[0] > minf[0]) minc[0] = minf[0] ;
             if (minc[1] > minf[1]) minc[1] = minf[1] ;
             if (minc[2] > minf[2]) minc[2] = minf[2] ;
            }
            range_result = QueryRangeTree(minc,maxc,&range_size) ;
	    found = 0 ;
            for (m=0; m < range_size; m++) {
             inte = adtreehash[range_result[m]] ;
	     if (inte == current) {
	      found = 1 ;
	      break ;
             }
            }
	    if (!found) {
             fprintf (stderr,"Face=%d %d %d for Face=%d %d %d not in AD_TREE\n",
		     current->verts[0], current->verts[1], current->verts[2],
		     face->verts[0],    face->verts[1],    face->verts[2]) ;
            }
	    QueryCleanRangeTree ( ) ;
#endif
            return 0;
         }
        }

        /* test to see if new point is near from current face. If it's
           then this point should not be considered */

        type = Msh3DTmp3( current, face, node_list, node_indx, debug );

	if( type == 0 ) return 0;
    }
#endif

    /* last check: see if there is any point inside the new element */

     type = Msh3DTmp4( face, node_indx );
     if ( type == 0 ) return 0 ;

    /* last new check: see if the faces that will be created were
       already in the list before */

#if OLD_TEST
     type = Msh3DTmp5( face, node_list, node_indx );
     if (type == 0) printf ("Tmp5 found\n") ;
     if ( type == 0 ) return 0 ;
#endif

    /* last new check: if the face is a boundary face and the node is a
       boundary node, the node is only valid if makes a good element to
       avoid future flat elements near the boundary, where nothing else
       can be done to correct them after the creation of the mesh. */

    if ( jmesh_version == SMALL_VERSION ) {
      type = Msh3DTmp9( face, node_list, node_indx, node_bdry );
      if ( type == 0 ) return 0 ;
    }

    /* check if one of the new faces will have null area, which means that
       a colinear node with one of the face's edges has been chosen. In this
       case, the node will not be accepted. */

#if OLD_NULL
    type = Msh3DTmp10( face, node_list, node_indx ) ;
#if DSP_PRNT
    if ( type == 0 ) fprintf( stderr, "Element with one face of null area\n");
#endif
    if ( type == 0 ) return 0 ;
#endif

    /* if we get here, the new element is valid */

#if DSP_PRNT
    if ( valid && print )
     fprintf(stderr,"Valid  insertion of new node\n" ) ;
#endif

    return(valid) ;
}
#else
static int  Msh3DCheckValid(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                debug,
    int                print,
    int               *elem,
    int               *nelem )
{
    int    i, j, k, l, m, n, valid, inter, nc ;
    double min[3], max[3] ;
    int    v1, v2, type, ind ;
    Msh3DBdryFace  current ;
    Msh3DBdryFace  inte ;
    Msh3DBdryFaceRec new ;
    double tol, len ;
    int    p[3], q[3] ;
    double a[3], b[3], c[3], u[3], v[3], w[3] ;
    int    fface_nodeid[3],    nface_nodeid[3] ;
    double fface_coords[3][3], nface_coords[3][3] ;
    double cnorm[3], fnorm[3], cd, fd, norm ;

    /* get the max and min coords for the proposed element */

    for ( i=0 ; i<3 ; i++ ) {
        min[i] = face->min[i] ;
        max[i] = face->max[i] ;
        if ( node_list[node_indx].coord[i] < min[i] )
            min[i] = node_list[node_indx].coord[i] ;
        if ( node_list[node_indx].coord[i] > max[i] )
            max[i] = node_list[node_indx].coord[i] ;
    }

    /* loop through all the faces in the current boundary and see
       if they intersect any of the faces of the proposed new element */

#if DSP_MESH
    if (FDEBUG && DebugFail ) {
       GdbSetTopOff (GDB_DELFACE);
       GdbSetTopOff (GDB_NEWFACE);
       GdbSetTopOff (GDB_VERTEX);
       GdbSetTopOff (GDB_EDGE);
       GdbClear();
    }
#endif

#if AD_TREE
    /* Check the intersection only against faces in the AD_TREE */

    /* Find max and min */
    minc[0] = face->min[0] ;
    minc[1] = face->min[1] ;
    minc[2] = face->min[2] ;
    maxc[0] = face->max[0] ;
    maxc[1] = face->max[1] ;
    maxc[2] = face->max[2] ;
    for (n=1 ; n<4 ; n++ ) {
     node_id[0] = face->verts[n-1] ;
     node_id[1] = face->verts[n%3] ;
     node_id[2] = node_indx ;
     for (l=0; l < 3; l++) {
      maxf[l] = node_list[node_id[0]].coord[l] ;
      minf[l] = node_list[node_id[0]].coord[l] ;
      if (maxf[l] < node_list[node_id[1]].coord[l])
       maxf[l] = node_list[node_id[1]].coord[l] ;
      if (minf[l] > node_list[node_id[1]].coord[l])
       minf[l] = node_list[node_id[1]].coord[l] ;
      if (maxf[l] < node_list[node_id[2]].coord[l])
       maxf[l] = node_list[node_id[2]].coord[l] ;
      if (minf[l] > node_list[node_id[2]].coord[l])
       minf[l] = node_list[node_id[2]].coord[l] ;
     }
     if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
     if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
     if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
     if (minc[0] > minf[0]) minc[0] = minf[0] ;
     if (minc[1] > minf[1]) minc[1] = minf[1] ;
     if (minc[2] > minf[2]) minc[2] = minf[2] ;
    }

#if AD_DEBU
    /* check the comparison between the number of faces that would
     * be checked with and without the use of the AD_TREE */
    curol = 0 ;
    Msh3DBdryReset() ;
    while ( current = Msh3DBdryNext() ) curol++ ;
    range_result = QueryRangeTree(minc,maxc,&range_size) ;
    curfa = range_size ;
    if (all_faces) {
     printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                face->verts[1],
 				                face->verts[2]) ;
     if (face->use == 0)
      printf("Geometry: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 1)
      printf("Topology: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     if (face->use == 2)
      printf("Tracking: Normal=%d Ad_Tree=%d\n",curol,curfa) ;
     printf ("\n") ;
    }
    if (big_faces) {
     if (curfa > curol) {
      printf("Face:     Nodes = %d   %d   %d\n", face->verts[0],
                                                 face->verts[1],
  				                 face->verts[2]) ;
      printf("Normal=%d Ad_Tree=%d\n",curol,curfa) ;
      printf ("\n") ;
     }
    }
    QueryCleanRangeTree ( ) ;
#endif

    /* Get the tolerance based on edge length */
    l1 = sqrt( face->r[0]*face->r[0] +
	       face->r[1]*face->r[1] +
	       face->r[2]*face->r[2] ) ;
    l2 = sqrt( face->s[0]*face->s[0] +
	       face->s[1]*face->s[1] +
	       face->s[2]*face->s[2] ) ;
    l3 = sqrt( face->t[0]*face->t[0] +
	       face->t[1]*face->t[1] +
	       face->t[2]*face->t[2] ) ;
    tol = ((l1 + l2 + l3)/3.0) * 0.01 ;

    /* Use AD Tree to speed up the intersection checking */
    valid = 1 ;
    range_result = QueryRangeTree(minc,maxc,&range_size) ;
    for (m=0; m < range_size; m++) {

	/* get the tolerance and the face to be checked */
        current = adtreehash[range_result[m]] ;
	if (current == 0) continue ;
        tol = current->area * tol_factor ;

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* test if the current face is the same of given face, because
           if so no intersection test is necessary. */

        if ( current == face )
            continue ;

	/* get the nodes (a,b,c), normalized normal (cnorm) and the
	 * constant for plane equation (cd) for the current face */

	fface_nodeid[0] = current->verts[0] ;
	fface_nodeid[1] = current->verts[1] ;
	fface_nodeid[2] = current->verts[2] ;
        for ( j = 0 ; j < 3 ; j++ ) {
         a[j] = node_list[current->verts[0]].coord[j] ;
         b[j] = node_list[current->verts[1]].coord[j] ;
         c[j] = node_list[current->verts[2]].coord[j] ;
	 fface_coords[0][j] = a[j] ;
	 fface_coords[1][j] = b[j] ;
	 fface_coords[2][j] = c[j] ;
         p[j] = 0 ;
        }
	TripleCrossProd (a,b,c,cnorm) ;
	len = sqrt (cnorm[0]*cnorm[0]+cnorm[1]*cnorm[1]+cnorm[2]*cnorm[2]) ;
	if (fabs(len) >= 1e-12) {
         cnorm[0] /= len ;
         cnorm[1] /= len ;
         cnorm[2] /= len ;
        }
	cd = cnorm[0]*a[0] + cnorm[1]*a[1] + cnorm[2]*a[2] ;

        /* test the intersection between the boundary triangle we are
           currently checking with the three new faces. */

        for ( k = 0 ; k < 3 ; k++ ) {

	 /* get the nodes (u,v,w), normalized normal (fnorm) and the
	  * constant for plane equation (fd) for the new face */

	 nface_nodeid[0] = face->verts[k] ;
	 nface_nodeid[1] = face->verts[(k+1)%3] ;
	 nface_nodeid[2] = node_indx ;
         for ( j = 0 ; j < 3 ; j++ ) {
          u[j] = node_list[face->verts[k]].coord[j] ;
          v[j] = node_list[face->verts[(k+1)%3]].coord[j] ;
          w[j] = node_list[node_indx].coord[j] ;
          q[j] = 0 ;
	  nface_coords[0][j] = u[j] ;
	  nface_coords[1][j] = v[j] ;
	  nface_coords[2][j] = w[j] ;
         }
	 TripleCrossProd (u,v,w,fnorm) ;
	 len = sqrt (fnorm[0]*fnorm[0]+fnorm[1]*fnorm[1]+fnorm[2]*fnorm[2]) ;
	 if (fabs(len) >= 1e-12) {
          fnorm[0] /= len ;
          fnorm[1] /= len ;
          fnorm[2] /= len ;
         }
 	 fd = fnorm[0]*u[0] + fnorm[1]*u[1] + fnorm[2]*u[2] ;

	 /* determine if the two faces are co-planar or not and then
	  * check if they intersect each other. */

	 nc = 0 ;
	 for (l=0 ; l<3 ; ++l) {
          if (l==0) {
           if ((a[0]==u[0])&&(a[1]==u[1])&&(a[2]==u[2])) ++nc ;
           if ((b[0]==u[0])&&(b[1]==u[1])&&(b[2]==u[2])) ++nc ;
           if ((c[0]==u[0])&&(c[1]==u[1])&&(c[2]==u[2])) ++nc ;
          }
          if (l==1) {
           if ((a[0]==v[0])&&(a[1]==v[1])&&(a[2]==v[2])) ++nc ;
           if ((b[0]==v[0])&&(b[1]==v[1])&&(b[2]==v[2])) ++nc ;
           if ((c[0]==v[0])&&(c[1]==v[1])&&(c[2]==v[2])) ++nc ;
          }
          if (l==2) {
           if ((a[0]==w[0])&&(a[1]==w[1])&&(a[2]==w[2])) ++nc ;
           if ((b[0]==w[0])&&(b[1]==w[1])&&(b[2]==w[2])) ++nc ;
           if ((c[0]==w[0])&&(c[1]==w[1])&&(c[2]==w[2])) ++nc ;
          }
         }
	 norm = (cnorm[0]*fnorm[0]+cnorm[1]*fnorm[1]+cnorm[2]*fnorm[2]) ;

	 if ((fabs(norm)>0.9994) &&
             ((nc == 2) || (fabs(fabs(fd)-fabs(cd)) < tol))) {
          if (FaceIntersectCoPlanar(nface_coords, nface_nodeid,
				    fface_coords, fface_nodeid)) {
           type = 0 ;
          }
	  else {
           type = 1 ;
          }
         }
	 else {
          if (FaceIntersect(nface_coords, nface_nodeid,
		            fface_coords, fface_nodeid)) {
           type = 0 ;
          }
	  else {
           type = 1 ;
          }
         }

         if( type == 0 ) {
#if DSP_MESH
            if ( FDEBUG && DebugFail )
              GdbDspFail (node_indx,v1,v2);
#endif
#if AD_DEBU
	    /* check to see if intersected face is in AD_TREE */
	    found = 0 ;
            for (m=0; m < range_size; m++) {
             inte = adtreehash[range_result[m]] ;
	     if (inte == current) {
	      found = 1 ;
	      break ;
             }
            }
	    if (!found) {
             fprintf (stderr,"Face=%d %d %d for Face=%d %d %d not in AD_TREE\n",
		     current->verts[0], current->verts[1], current->verts[2],
		     face->verts[0],    face->verts[1],    face->verts[2]) ;
            }
#endif
	    QueryCleanRangeTree ( ) ;
            return 0;
         }
        }

        /* test to see if new point is near from current face. If it's
           then this point should not be considered */

        type = Msh3DTmp3( current, face, node_list, node_indx, debug );

	if( type == 0 ) {
	 QueryCleanRangeTree ( ) ;
	 return 0;
        }
    }
    QueryCleanRangeTree ( ) ;
#else
    /* Get the tolerance based on edge length */
    l1 = sqrt( face->r[0]*face->r[0] +
	       face->r[1]*face->r[1] +
	       face->r[2]*face->r[2] ) ;
    l2 = sqrt( face->s[0]*face->s[0] +
	       face->s[1]*face->s[1] +
	       face->s[2]*face->s[2] ) ;
    l3 = sqrt( face->t[0]*face->t[0] +
	       face->t[1]*face->t[1] +
	       face->t[2]*face->t[2] ) ;
    tol = ((l1 + l2 + l3)/3.0) * 0.01 ;

    /* Check intersection against all faces on the boundary */
    valid = 1 ;
    Msh3DBdryReset() ;
    while ( valid && (current = Msh3DBdryNext()) ) {

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* test if the current face is the same of given face, because
           if so no intersection test is necessary. */

        if ( current == face )
            continue ;

	/* get the nodes (a,b,c), normalized normal (cnorm) and the
	 * constant for plane equation (cd) for the current face */

	fface_nodeid[0] = current->verts[0] ;
	fface_nodeid[1] = current->verts[1] ;
	fface_nodeid[2] = current->verts[2] ;
        for ( j = 0 ; j < 3 ; j++ ) {
         a[j] = node_list[current->verts[0]].coord[j] ;
         b[j] = node_list[current->verts[1]].coord[j] ;
         c[j] = node_list[current->verts[2]].coord[j] ;
	 fface_coords[0][j] = a[j] ;
	 fface_coords[1][j] = b[j] ;
	 fface_coords[2][j] = c[j] ;
         p[j] = 0 ;
        }
	TripleCrossProd (a,b,c,cnorm) ;
	len = sqrt (cnorm[0]*cnorm[0]+cnorm[1]*cnorm[1]+cnorm[2]*cnorm[2]) ;
	if (fabs(len) >= 1e-12) {
         cnorm[0] /= len ;
         cnorm[1] /= len ;
         cnorm[2] /= len ;
        }
	cd = cnorm[0]*a[0] + cnorm[1]*a[1] + cnorm[2]*a[2] ;

        /* test the intersection between the boundary triangle we are
           currently checking with the three new faces. */

        for ( k = 0 ; k < 3 ; k++ ) {

	 /* get the nodes (u,v,w), normalized normal (fnorm) and the
	  * constant for plane equation (fd) for the new face */

	 nface_nodeid[0] = face->verts[k] ;
	 nface_nodeid[1] = face->verts[(k+1)%3] ;
	 nface_nodeid[2] = node_indx ;
         for ( j = 0 ; j < 3 ; j++ ) {
          u[j] = node_list[face->verts[k]].coord[j] ;
          v[j] = node_list[face->verts[(k+1)%3]].coord[j] ;
          w[j] = node_list[node_indx].coord[j] ;
          q[j] = 0 ;
	  nface_coords[0][j] = u[j] ;
	  nface_coords[1][j] = v[j] ;
	  nface_coords[2][j] = w[j] ;
         }
	 TripleCrossProd (u,v,w,fnorm) ;
	 len = sqrt (fnorm[0]*fnorm[0]+fnorm[1]*fnorm[1]+fnorm[2]*fnorm[2]) ;
	 if (fabs(len) >= 1e-12) {
          fnorm[0] /= len ;
          fnorm[1] /= len ;
          fnorm[2] /= len ;
         }
 	 fd = fnorm[0]*u[0] + fnorm[1]*u[1] + fnorm[2]*u[2] ;

	 /* determine if the two faces are co-planar or not and then
	  * check if they intersect each other. */

	 nc = 0 ;
	 for (l=0 ; l<3 ; ++l) {
          if (l==0) {
           if ((a[0]==u[0])&&(a[1]==u[1])&&(a[2]==u[2])) ++nc ;
           if ((b[0]==u[0])&&(b[1]==u[1])&&(b[2]==u[2])) ++nc ;
           if ((c[0]==u[0])&&(c[1]==u[1])&&(c[2]==u[2])) ++nc ;
          }
          if (l==1) {
           if ((a[0]==v[0])&&(a[1]==v[1])&&(a[2]==v[2])) ++nc ;
           if ((b[0]==v[0])&&(b[1]==v[1])&&(b[2]==v[2])) ++nc ;
           if ((c[0]==v[0])&&(c[1]==v[1])&&(c[2]==v[2])) ++nc ;
          }
          if (l==2) {
           if ((a[0]==w[0])&&(a[1]==w[1])&&(a[2]==w[2])) ++nc ;
           if ((b[0]==w[0])&&(b[1]==w[1])&&(b[2]==w[2])) ++nc ;
           if ((c[0]==w[0])&&(c[1]==w[1])&&(c[2]==w[2])) ++nc ;
          }
         }
	 norm = (cnorm[0]*fnorm[0]+cnorm[1]*fnorm[1]+cnorm[2]*fnorm[2]) ;

	 if ((fabs(norm)>0.9994) &&
             ((nc == 2) || (fabs(fabs(fd)-fabs(cd)) < tol))) {
          if (FaceIntersectCoPlanar(nface_coords, nface_nodeid,
				    fface_coords, fface_nodeid)) {
           type = 0 ;
          }
	  else {
           type = 1 ;
          }
         }
	 else {
          if (FaceIntersect(nface_coords, nface_nodeid,
		            fface_coords, fface_nodeid)) {
           type = 0 ;
          }
	  else {
           type = 1 ;
          }
         }

         if( type == 0 ) {
#if DSP_MESH
            if ( FDEBUG && DebugFail )
              GdbDspFail (node_indx,v1,v2);
#endif
#if AD_DEBU
	    /* check to see if intersected face is in AD_TREE */
            minc[0] = face->min[0] ;
            minc[1] = face->min[1] ;
            minc[2] = face->min[2] ;
            maxc[0] = face->max[0] ;
            maxc[1] = face->max[1] ;
            maxc[2] = face->max[2] ;
            for (n=1 ; n<4 ; n++ ) {
             node_id[0] = face->verts[n-1] ;
             node_id[1] = face->verts[n%3] ;
             node_id[2] = node_indx ;
             for (l=0; l < 3; l++) {
              maxf[l] = node_list[node_id[0]].coord[l] ;
              minf[l] = node_list[node_id[0]].coord[l] ;
              if (maxf[l] < node_list[node_id[1]].coord[l])
               maxf[l] = node_list[node_id[1]].coord[l] ;
              if (minf[l] > node_list[node_id[1]].coord[l])
               minf[l] = node_list[node_id[1]].coord[l] ;
              if (maxf[l] < node_list[node_id[2]].coord[l])
               maxf[l] = node_list[node_id[2]].coord[l] ;
              if (minf[l] > node_list[node_id[2]].coord[l])
               minf[l] = node_list[node_id[2]].coord[l] ;
             }
             if (maxc[0] < maxf[0]) maxc[0] = maxf[0] ;
             if (maxc[1] < maxf[1]) maxc[1] = maxf[1] ;
             if (maxc[2] < maxf[2]) maxc[2] = maxf[2] ;
             if (minc[0] > minf[0]) minc[0] = minf[0] ;
             if (minc[1] > minf[1]) minc[1] = minf[1] ;
             if (minc[2] > minf[2]) minc[2] = minf[2] ;
            }
            range_result = QueryRangeTree(minc,maxc,&range_size) ;
	    found = 0 ;
            for (m=0; m < range_size; m++) {
             inte = adtreehash[range_result[m]] ;
	     if (inte == current) {
	      found = 1 ;
	      break ;
             }
            }
	    if (!found) {
             fprintf (stderr,"Face=%d %d %d for Face=%d %d %d not in AD_TREE\n",
		     current->verts[0], current->verts[1], current->verts[2],
		     face->verts[0],    face->verts[1],    face->verts[2]) ;
            }
	    QueryCleanRangeTree ( ) ;
#endif
            return 0;
         }
        }

        /* test to see if new point is near from current face. If it's
           then this point should not be considered */

        type = Msh3DTmp3( current, face, node_list, node_indx, debug );

	if( type == 0 ) return 0;
    }
#endif

    /* last check: see if there is any point inside the new element */

     type = Msh3DTmp4( face, node_indx );
     if ( type == 0 ) return 0 ;

    /* last new check: see if the faces that will be created were
       already in the list before */

#if OLD_TEST
     type = Msh3DTmp5( face, node_list, node_indx );
     if (type == 0) printf ("Tmp5 found\n") ;
     if ( type == 0 ) return 0 ;
#endif

    /* last new check: if the face is a boundary face and the node is a
       boundary node, the node is only valid if makes a good element to
       avoid future flat elements near the boundary, where nothing else
       can be done to correct them after the creation of the mesh. */

    if ( jmesh_version == SMALL_VERSION ) {
      type = Msh3DTmp9( face, node_list, node_indx, node_bdry );
      if ( type == 0 ) return 0 ;
    }

    /* check if one of the new faces will have null area, which means that
       a colinear node with one of the face's edges has been chosen. In this
       case, the node will not be accepted. */

#if OLD_NULL
    type = Msh3DTmp10( face, node_list, node_indx ) ;
    if ( type == 0 ) fprintf( stderr, "element with one face of null area\n");
    if ( type == 0 ) return 0 ;
#endif

    /* if we get here, the new element is valid */

#if DSP_PRNT
    if ( valid && print )
     fprintf(stderr,"Valid  insertion of new node\n" ) ;
#endif

    return(valid) ;
}
#endif

static int  Msh3DTmp1(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                v1,
    int                v2 )
{
     int    i;
     int    common, ncom;
     double tprod1, tprod2, dot, side, scal ;
     double *c1, *c2, *c3, *c0, *ci, *cj ;
     double r[3],s[3],t[3],r_x_t[3],s_x_t[3],r_x_s[3] ;
     double tol ;

     /* get the tolerance based in face area */

     tol = face->area * tol_factor ;

     /* check to see if both vertices are common to the face */

     for ( i = 0 ; i<3 ; i++ ) {
        if (  v1 == face->verts[i] &&
              v2 == face->verts[(i+1)%3] )
          return 1 ;
     }

     /* check to see if any of the vertices are common to the face */

     for ( i=0, common=(-1) ; i<3 ; i++ ) {
       if ( v1 == face->verts[i] ) {
         ncom = v2;
         common = i ;
         break ;
       } else if ( v2 == face->verts[i] ) {
         ncom = v1;
         common = i ;
         break ;
       }

     }

     if ( common > -1 ) {

     /* compute the triple product of the nodal coordinate dotted
        with the r cross s vector for the face.  If this value is
        not equal to zero, then the vertex does not lie in the plane
        of the triangle */

        tprod1 = 0.0 ;
        c0 = node_list[ncom].coord ;
        for ( i=0 ; i<3 ; i++ ) tprod1 += c0[i] * face->r_x_s[i] ;
        tprod1 -= face->rs_dot_v1 ;

        if( 0 ) fprintf( stderr, "tprod1=\t%lf\n", tprod1 ) ;
        if ( !ABOUT_ZERO(tprod1,tol) ) return 1 ;

     /* if we get here the other vertex lies in the plane of the triangle.
        we need to see if the new edge will cross on of the edges of the
        triangle.  To check this we first cross one edge of the triangle
        with the proposed new edges, and then cross the other.  These
        cross product vectors should be parellel.  If they have the same
        sign, the edge is OK.  If not we reject the new element. Of course
        we always have to test if the vectors are coincident or not, using
        the dot product and considering the crack faces. */

        c1 = node_list[face->verts[common]].coord ;
        c2 = node_list[face->verts[(common+1)%3]].coord ;
        c3 = node_list[face->verts[(common+2)%3]].coord ;

        for ( i=0 ; i<3 ; i++ ) {
          r[i] = c2[i] - c1[i] ;
          s[i] = c3[i] - c1[i] ;
          t[i] = c0[i] - c1[i] ;
        }

        s_x_t[0] = s[1] * t[2] - t[1] * s[2] ;
        s_x_t[1] = s[2] * t[0] - t[2] * s[0] ;
        s_x_t[2] = s[0] * t[1] - t[0] * s[1] ;

        dot  = s[0]*t[0] + s[1]*t[1] + s[2]*t[2] ;
        dot /= sqrt( s[0]*s[0] + s[1]*s[1] + s[2]*s[2] ) ;
        dot /= sqrt( t[0]*t[0] + t[1]*t[1] + t[2]*t[2] ) ;
        dot -= 1.0 ;
        if ( ABOUT_ZERO(dot,tol) )
         if ( !(s[0] == t[0] && s[1] == t[1] && s[2] == t[2]) ) /* crack */
          return 0 ;

        r_x_t[0] = r[1] * t[2] - t[1] * r[2] ;
        r_x_t[1] = r[2] * t[0] - t[2] * r[0] ;
        r_x_t[2] = r[0] * t[1] - t[0] * r[1] ;

        dot  = r[0]*t[0] + r[1]*t[1] + r[2]*t[2] ;
        dot /= sqrt( r[0]*r[0] + r[1]*r[1] + r[2]*r[2] ) ;
        dot /= sqrt( t[0]*t[0] + t[1]*t[1] + t[2]*t[2] ) ;
        dot -= 1.0 ;
        if ( ABOUT_ZERO(dot,tol) )
         if ( !(r[0] == t[0] && r[1] == t[1] && r[2] == t[2]) ) /* crack */
          return 0 ;

        dot = s_x_t[0]*r_x_t[0] + s_x_t[1]*r_x_t[1] + s_x_t[2]*r_x_t[2] ;

        if ( dot < (-tol) ) {
	 r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
         r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
         r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
         dot = r_x_t[0]*r_x_s[0] + r_x_t[1]*r_x_s[1] +
         r_x_t[2]*r_x_s[2] ;
         return (dot < (-tol)) ;
        }
        else return 1;
     }

     return 2;
}

static int  Msh3DTmp2(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                v1,
    int                v2 )
{
    int    i;
    double tprod1, tprod2, side, scal ;
    double *c1, *c2, *c3, *ci, *cj ;
    double u, v, w, aux ;
    double t[3],r_x_t[3],s_x_t[3] ;
    double tol ;

    tol = face->area * tol_factor ;

    c1 = node_list[v1].coord ;
    c2 = node_list[v2].coord ;

    tprod1 = tprod2 = 0.0 ;
    for ( i=0 ; i<3 ; i++ ) {
      tprod1 += c1[i] * face->r_x_s[i] ;
      tprod2 += c2[i] * face->r_x_s[i] ;
    }
    tprod1 -= face->rs_dot_v1 ;
    tprod2 -= face->rs_dot_v1 ;

    if (( tprod1*tprod2 ) > (-tol) ) return 1 ;

    /* if we get here then the two end points of the edge are
       on opposite sides of the face.  We then compute the
       parametric coordinates of the intersection of the edge
       with the face's plane, and check to see if they are
       in the range 0 - 1 */

    t[0] = c2[0]-c1[0] ; t[1] = c2[1]-c1[1] ; t[2] = c2[2]-c1[2] ;
    s_x_t[0] = face->s[1] * t[2] - t[1] * face->s[2] ;
    s_x_t[1] = face->s[2] * t[0] - t[2] * face->s[0] ;
    s_x_t[2] = face->s[0] * t[1] - t[0] * face->s[1] ;

    c3 = node_list[face->verts[0]].coord ;
    u  = s_x_t[0]*c1[0] + s_x_t[1]*c1[1] + s_x_t[2]*c1[2] ;
    u -= s_x_t[0]*c3[0] + s_x_t[1]*c3[1] + s_x_t[2]*c3[2] ;
    aux = s_x_t[0]*face->r[0] + s_x_t[1]*face->r[1] +
          s_x_t[2]*face->r[2] ;
   /* if (fabs(aux)>1e-07) { */
      u /= aux;
      if ( OUT_FACE(u,tol_factor)) return 1 ;

    r_x_t[0] = face->r[1] * t[2] - t[1] * face->r[2] ;
    r_x_t[1] = face->r[2] * t[0] - t[2] * face->r[0] ;
    r_x_t[2] = face->r[0] * t[1] - t[0] * face->r[1] ;
    v  = r_x_t[0]*c1[0] + r_x_t[1]*c1[1] + r_x_t[2]*c1[2] ;
    v -= r_x_t[0]*c3[0] + r_x_t[1]*c3[1] + r_x_t[2]*c3[2] ;
    aux = r_x_t[0]*face->s[0] + r_x_t[1]*face->s[1] +
          r_x_t[2]*face->s[2] ;
    if (fabs(aux)>1e-07)
    {
      v /= aux;
      if ( OUT_FACE(v,tol_factor)) return 1 ;
    }

    w = 1.0 - u - v ;
    if ( OUT_FACE(w,tol_factor)) return 1 ;

    /* if we get here, then the edge crosses the face
       so we should return with an invalid status */

    return  0 ;
}

static int Msh3DTmp3(
  Msh3DBdryFace      face,
  Msh3DBdryFace      bface,
  Msh3DBdryNodeList  node_list,
  int                node_indx,
  int                debug )
{
  Msh3DBdryFace facef ;
  int           i , factor = 1 ;
  double        *c, *c1, *c2, *c3 ;
  double        cand_vec[3], proj_vec[3], proj[3] ;
  double        r[3], s[3], r_x_s[3], area, u, v, w ;
  double        dot, edge, param, height, num, den, dist ;

  /* this test shouldn't be done if node choosen is not a new node created
     by an internal adjacent procedure */

  if ( debug == -1 )
   return 1 ;

  c  = node_list[node_indx].coord ;
  c1 = node_list[face->verts[0]].coord ;
  c2 = node_list[face->verts[1]].coord ;
  c3 = node_list[face->verts[2]].coord ;


  /* we should verify if the distance from the new node to the plane of  the
     face is bigger than the height that should be used to generate the best
     element we could divided by a factor (the best element is  the  element
     with the four faces more equal to a equilateral triangle than possible) */

  edge = sqrt( (4*face->area) / sqrt(3.0) );
  height = (edge * sqrt(13.0)) / 4.0;
  cand_vec[0] = c[0] - face->center[0] ;
  cand_vec[1] = c[1] - face->center[1] ;
  cand_vec[2] = c[2] - face->center[2] ;
  num = cand_vec[0] * face->r_x_s[0] +
        cand_vec[1] * face->r_x_s[1] +
        cand_vec[2] * face->r_x_s[2] ;
  den = face->r_x_s[0] * face->r_x_s[0] +
        face->r_x_s[1] * face->r_x_s[1] +
        face->r_x_s[2] * face->r_x_s[2] ;
  dist = fabs(num) / sqrt(den) ;
  if ( dist > (height/(SPH_FACTOR*factor)) )
   return 1 ;

  /* if we get here the distance from the new node to the face's plane is not
     bigger than the height of the new element divided by a factor. We should
     then evaluate the projection of the new node into  the  face's plane and
     verify if this point is inside of the face or near of it to reject  this
     new node. The projection of the new node is computed  by a  dot product
     between a vector formed by the new node and the face's center  and  the
     normal vector of the face and to evaluate the projection is used baricen-
     tric coordinates */

  r_x_s[0] = face->r_x_s[0] / sqrt(den) ;
  r_x_s[1] = face->r_x_s[1] / sqrt(den) ;
  r_x_s[2] = face->r_x_s[2] / sqrt(den) ;
  dot = cand_vec[0] * r_x_s[0] +
        cand_vec[1] * r_x_s[1] +
        cand_vec[2] * r_x_s[2] ;
  r_x_s[0] *= dot ;
  r_x_s[1] *= dot ;
  r_x_s[2] *= dot ;
  proj_vec[0] = cand_vec[0] - r_x_s[0] ;
  proj_vec[1] = cand_vec[1] - r_x_s[1] ;
  proj_vec[2] = cand_vec[2] - r_x_s[2] ;
  proj[0] = proj_vec[0] + face->center[0] ;
  proj[1] = proj_vec[1] + face->center[1] ;
  proj[2] = proj_vec[2] + face->center[2] ;

  r[0] = c2[0]-proj[0] ; r[1] = c2[1]-proj[1] ; r[2] = c2[2]-proj[2] ;
  s[0] = c3[0]-proj[0] ; s[1] = c3[1]-proj[1] ; s[2] = c3[2]-proj[2] ;
  r_x_s[0] = r[1]*s[2] - s[1]*r[2] ;
  r_x_s[1] = r[2]*s[0] - s[2]*r[0] ;
  r_x_s[2] = r[0]*s[1] - s[0]*r[1] ;
  area = sqrt( r_x_s[0] * r_x_s[0] +
	       r_x_s[1] * r_x_s[1] +
	       r_x_s[2] * r_x_s[2] ) / 2.0 ;
  u = area / face->area ;
  dot = r_x_s[0] * face->r_x_s[0] +
        r_x_s[1] * face->r_x_s[1] +
        r_x_s[2] * face->r_x_s[2] ;
  u *= SIGN(dot) ;

  r[0] = c3[0]-proj[0] ; r[1] = c3[1]-proj[1] ; r[2] = c3[2]-proj[2] ;
  s[0] = c1[0]-proj[0] ; s[1] = c1[1]-proj[1] ; s[2] = c1[2]-proj[2] ;
  r_x_s[0] = r[1]*s[2] - s[1]*r[2] ;
  r_x_s[1] = r[2]*s[0] - s[2]*r[0] ;
  r_x_s[2] = r[0]*s[1] - s[0]*r[1] ;
  area = sqrt( r_x_s[0] * r_x_s[0] +
	       r_x_s[1] * r_x_s[1] +
	       r_x_s[2] * r_x_s[2] ) / 2.0 ;
  v = area / face->area ;
  dot = r_x_s[0] * face->r_x_s[0] +
        r_x_s[1] * face->r_x_s[1] +
        r_x_s[2] * face->r_x_s[2] ;
  v *= SIGN(dot) ;

  w = 1.0 - u - v ;

  if (u < 0.0 && v > 0.0 && w > 0.0)
  {
   edge = sqrt ( (c2[0]-c3[0]) * (c2[0]-c3[0]) +
		 (c2[1]-c3[1]) * (c2[1]-c3[1]) +
		 (c2[2]-c3[2]) * (c2[2]-c3[2]) ) ;
   param = (face->area * 2.0) / edge;
   dist = param * fabs(u);
   if ( dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }
  else if (u > 0.0 && v < 0.0 && w > 0.0)
  {
   edge = sqrt ( (c1[0]-c3[0]) * (c1[0]-c3[0]) +
		 (c1[1]-c3[1]) * (c1[1]-c3[1]) +
		 (c1[2]-c3[2]) * (c1[2]-c3[2]) ) ;
   param = (face->area * 2.0) / edge;
   dist = param * fabs(v);
   if (dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }
  else if (u > 0.0 && v > 0.0 && w < 0.0)
  {
   edge = sqrt ( (c1[0]-c2[0]) * (c1[0]-c2[0]) +
		 (c1[1]-c2[1]) * (c1[1]-c2[1]) +
		 (c1[2]-c2[2]) * (c1[2]-c2[2]) ) ;
   param = (face->area * 2.0) / edge;
   dist = param * fabs(w);
   if (dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }

  if (u < 0.0 && v < 0.0 && w > 0.0)
  {
   dist = sqrt ( (c[0]-c3[0]) * (c[0]-c3[0]) +
		 (c[1]-c3[1]) * (c[1]-c3[1]) +
		 (c[2]-c3[2]) * (c[2]-c3[2]) ) ;
   if (dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }
  else if (u < 0.0 && v > 0.0 && w < 0.0)
  {
   dist = sqrt ( (c[0]-c2[0]) * (c[0]-c2[0]) +
		 (c[1]-c2[1]) * (c[1]-c2[1]) +
		 (c[2]-c2[2]) * (c[2]-c2[2]) ) ;
   if (dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }
  else if (u > 0.0 && v < 0.0 && w < 0.0)
  {
   dist = sqrt ( (c[0]-c1[0]) * (c[0]-c1[0]) +
		 (c[1]-c1[1]) * (c[1]-c1[1]) +
		 (c[2]-c1[2]) * (c[2]-c1[2]) ) ;
   if (dist > (height/(SPH_FACTOR*factor)))
    return 1 ;
  }

  /* if we get here, then distance is not bigger than height divided by a fac-
     tor and the projection of the new node in the plane of the face is inside
     of the face or near of it, then we should return a invalid status. */

   return 0 ;
}

static double PtDistPlane (int id, double n[3], double p[3] )
{
  int   i;
  double dist = 0.0, D = 0.0;
  for(i=0;i<3;i++)
    D -= n[i]*node_list[id].coord[i];

  for(i=0;i<3;i++)
    dist += n[i]*p[i];
  return (dist += D);
}

static int PtBelowHalfPlane (int id, double n[3], double p[3] )
{
  double d;
  d = PtDistPlane(id, n, p);
  return ( d < 0 );
}

static int PtAboveHalfPlane (int id, double n[3], double p[3] )
{
  double d;
  d = PtDistPlane(id, n, p);
  return ( d > 0 );
}

static int PtInsideElement ( Msh3DBdryFace face , int node_indx, double p[3] )
{
  int            i, j;
  double         n[3];
  Msh3DBdryFaceRec  new ;

  if ( PtBelowHalfPlane( face->verts[0], face->r_x_s, p )) return 0;

/* now we need to see if the edges of the boundary triangle intersect
   any of the three new faces */
/* compute the information we need for the new face */

  for ( i=0 ; i<3 ; i++ )  /* loop over new faces */
  {
    new.verts[0] = face->verts[i] ;
    new.verts[1] = face->verts[(i+1)%3] ;
    new.verts[2] = node_indx ;

/* compute the r and s vectors and cross products */

    for ( j=0 ; j<3 ; j++ )
    {
       new.r[j] = node_list[new.verts[1]].coord[j] -
                  node_list[new.verts[0]].coord[j] ;
       new.s[j] = node_list[new.verts[2]].coord[j] -
                  node_list[new.verts[0]].coord[j] ;
    }
    new.r_x_s[0] = new.r[1] * new.s[2] - new.s[1] * new.r[2] ;
    new.r_x_s[1] = new.r[2] * new.s[0] - new.s[2] * new.r[0] ;
    new.r_x_s[2] = new.r[0] * new.s[1] - new.s[0] * new.r[1] ;
    if ( PtAboveHalfPlane( new.verts[0], new.r_x_s, p ) ) return 0;
  }
  return 1;
}

static int Msh3DTmp4( Msh3DBdryFace face, int node_indx )
{
  int          i, j, v1, v2, vn, vf;
  Msh3DAdjFace cur ;

  for ( i=0; i<3; i++ )
  {
    v1 = face->verts[(i+1)%3] ;
    v2 = face->verts[i] ;
    for ( cur=node_list[v2].faces ; cur ; cur=cur->next )
    {
      for ( j=0 ; j<3 ; j++ )
      {
        if (( cur->face->verts[j] == v1 ) &&
            ( cur->face->verts[(j+1)%3] == v2 ) &&
              cur->face->verts[(j+2)%3] != node_indx )
        {
          vn = cur->face->verts[(j+2)%3] ;
          vf = face->verts[(i+2)%3] ;
          if( (node_list[vn].coord[0] == node_list[vf].coord[0]) &&
              (node_list[vn].coord[1] == node_list[vf].coord[1]) &&
              (node_list[vn].coord[2] == node_list[vf].coord[2])  )
           continue ;  /* two nodes with same coordinates (crack) */

          if( PtInsideElement(face, node_indx,
              node_list[cur->face->verts[(j+2)%3]].coord) )
           return 0;
        }
      }
    }
  }
  return 1;
}

static int Msh3DTmp5(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int            v1i, v2i, v1, v2, i, j ;
    int            found, made ;
    Msh3DAdjFace   cur ;
    Msh3DBdryFace  fac ;

    /* start with each of the base nodes.  See if they are adjacent
       to a face that includes the cap node along with the previous
       vertex on the base.  If so, this face is already part of the
       boundary, and it should be deleted. If not, it will be created
       and we should verify if it was already created before. */

    for ( v1i=1,v2i=0 ; v2i<3 ; v1i=((v1i+1)%3), v2i++ ) {
        v1 = face->verts[v1i] ;
        v2 = face->verts[v2i] ;
        for ( cur=node_list[v1].faces ; cur ; cur=cur->next ) {
            for ( i=found=0 ; (i<3) && !found ; i++ ) {
                if (( cur->face->verts[i] == v1 ) &&
                    ( cur->face->verts[(i+1)%3] == v2 ) &&
                    ( cur->face->verts[(i+2)%3] == node_indx ))
                    found = 1 ;
            }
            if ( found ) break ;
        }

       /* if we haven't found a face, it means that this face will be
          created. Verify if this face was in the list anytime before
          to avoid creation of repeated faces again */

        if ( !found ) {
          made = 0 ;
	  fac = test_cursor ;
	  while( fac ) {
           if (( fac->verts[0] == v2 ) &&
               ( fac->verts[1] == v1 ) &&
               ( fac->verts[2] == node_indx ))
            made = 1 ;
           if ( made ) break ;
	   fac = fac->next ;
          }
          if ( made ) return 0 ;
        }
    }

    /* if we get here this face is valid, there is, it will create
       new faces never created before */

    return 1 ;
}

static int  Msh3DTmp6(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                vert1,
    int                vert2 )
{
    int                i, j ;
    int                i0, i1, i2 ;
    int                inter, n = 3, interpolate = 1 ;
    double             O[3], D[3] ;
    double             d, N[3], V[3][3], P[3] ;
    double             f1, f2, t ;
    double             u0, u1, u2, v0, v1, v2 ;
    double             alpha, beta, gamma ;
    double             rayP[3], rayN[3], NV[3][3] ;

    /* compute the ray for the edge */

    O[0] = node_list[vert1].coord[0] ;
    O[1] = node_list[vert1].coord[1] ;
    O[2] = node_list[vert1].coord[2] ;
    D[0] = (node_list[vert2].coord[0] - node_list[vert1].coord[0]) ;
    D[1] = (node_list[vert2].coord[1] - node_list[vert1].coord[1]) ;
    D[2] = (node_list[vert2].coord[2] - node_list[vert1].coord[2]) ;

    /* compute the polygon and the plane for the face */

    for ( i = 0; i < 3; i++ )
     for ( j = 0; j < 3; j++ )
      V[i][j] = node_list[face->verts[i]].coord[j] ;
    N[0] = face->r_x_s[0] ;
    N[1] = face->r_x_s[1] ;
    N[2] = face->r_x_s[2] ;
    d    = -1.0 * (node_list[face->verts[0]].coord[0] * face->r_x_s[0] +
                   node_list[face->verts[0]].coord[1] * face->r_x_s[1] +
                   node_list[face->verts[0]].coord[2] * face->r_x_s[2] ) ;

    /* compute the parameter t for the intersection point */

    f1 = (N[0]*O[0] + N[1]*O[1] + N[2]*O[2]) ;
    f2 = (N[0]*D[0] + N[1]*D[1] + N[2]*D[2]) ;
    if ( ABOUT_ZERO(f2,TOL_FACTOR) ) return 1 ;
    t  = -1.0 * ((d + f1) / f2) ;
    if ( (t < 0.0) || ABOUT_ZERO(t,TOL_FACTOR) || t >= 1.0 ) return 1 ;

    /* find the indices i0, i1 and i2 for the polygon */

    if ( (fabs(N[0]) >= fabs(N[1])) && (fabs(N[0]) >= fabs(N[2])) )
    {
     i0 = 0 ;
     i1 = 1 ;
     i2 = 2 ;
    }
    else if ( (fabs(N[1]) >= fabs(N[0])) && (fabs(N[1]) >= fabs(N[2])) )
    {
     i0 = 1 ;
     i1 = 2 ;
     i2 = 0 ;
    }
    else if ( (fabs(N[2]) >= fabs(N[0])) && (fabs(N[2]) >= fabs(N[1])) )
    {
     i0 = 2 ;
     i1 = 0 ;
     i2 = 1 ;
    }

    /* verify if the ray for the edge intersects the plane for the face */

    P[0] = O[0] + D[0]*t ;
    P[1] = O[1] + D[1]*t ;
    P[2] = O[2] + D[2]*t ;
    u0 = P[i1] - V[0][i1] ; v0 = P[i2] - V[0][i2] ;
    inter = FALSE ; i = 2 ;
    do {
        /* The polygon is viewed as (n-2) triangles. */

        u1 = V[i-1][i1] - V[0][i1] ; v1 = V[i-1][i2] - V[0][i2] ;
        u2 = V[i  ][i1] - V[0][i1] ; v2 = V[i  ][i2] - V[0][i2] ;

        if (u1 == 0) {
         beta = u0/u2 ;
         if ((beta >= 0.)&&(beta <= 1.)) {
          alpha = (v0 - beta*v2)/v1 ;
          inter = ((alpha >= 0.)&&((alpha+beta) <= 1.)) ;
         }
        }
        else {
         beta = (v0*u1 - u0*v1)/(v2*u1 - u2*v1) ;
         if ((beta >= 0.)&&(beta <= 1.)) {
          alpha = (u0 - beta*u2)/u1 ;
          inter = ((alpha >= 0)&&((alpha+beta) <= 1.)) ;
         }
        }
    } while ((!inter)&&(++i < n)) ;

    /* find the intersection point and normal - here it's not necessary */

    if ( 0 ) {
     if ( inter ) {            /* intersection  point */
      rayP[0] = P[0] ; rayP[1] = P[1] ; rayP[2] = P[2] ;
      if ( interpolate ) {     /* interpolation normal */
       gamma = 1 - (alpha+beta) ;
       rayN[0] = gamma * NV[0][0] + alpha * NV[i-1][0] + beta * NV[i][0] ;
       rayN[1] = gamma * NV[0][1] + alpha * NV[i-1][1] + beta * NV[i][1] ;
       rayN[2] = gamma * NV[0][2] + alpha * NV[i-1][2] + beta * NV[i][2] ;
      }
     }
    }

    if ( 0 /* inter */ )  {
     for ( i = 0; i < 3; i++ ) {
      rayP[0] = node_list[face->verts[i]].coord[0] ;
      rayP[1] = node_list[face->verts[i]].coord[1] ;
      rayP[2] = node_list[face->verts[i]].coord[2] ;
      if ( P[0] == rayP[0] && P[1] == rayP[1] && P[2] == rayP[2] )
       return 1 ;
     }
    }

    /* return the result of the intersection */

    if ( inter ) return 0 ;
    else         return 1 ;
}

static int  Msh3DTmp7(
    int                dir,
    double             a[3],
    double             b[3],
    double             c[3],
    int                p[3],
    double             u[3],
    double             v[3],
    double             w[3],
    int                q[3] )
{
    int                type ;

    /* test intersection against two given triangles and return if
       intersection that it's not coincident with any vertices is
       found */

    type = GeoTriIntersect (dir, a, b, c, p, u, v, w, q) ;
    if (type == 0) return 0 ;

   /* if we get here no intersection was found */

   return 1 ;
}

static int Msh3DTmp8Cramer(
    int               n,
    double            a[3][3],
    double            b[3],
    double            c[3] )
{
    double det, det0, det1, det2, tol_cramer = 5.0e-5 ;

    if ( n == 2 ) {
     det = a[0][0]*a[1][1] - a[0][1]*a[1][0] ;

     if (fabs(det)<tol_cramer) return 0 ;

     det0 = b[0]*a[1][1] - b[1]*a[0][1] ;
     det1 = a[0][0]*b[1] - a[1][0]*b[0] ;
     c[0] = det0/det ;
     c[1] = det1/det ;
    }
    else if ( n == 3 ) {
     det = a[0][0]*a[1][1]*a[2][2] +
	   a[1][0]*a[2][1]*a[0][2] +
	   a[2][0]*a[0][1]*a[1][2] -
           a[2][0]*a[1][1]*a[0][2] -
	   a[0][0]*a[2][1]*a[1][2] -
	   a[1][0]*a[0][1]*a[2][2] ;

     if (fabs(det)<tol_cramer) return 0 ;

     det0 = b[0]*a[1][1]*a[2][2]+b[1]*a[2][1]*a[0][2]+b[2]*a[0][1]*a[1][2]-
            b[2]*a[1][1]*a[0][2]-b[0]*a[2][1]*a[1][2]-b[1]*a[0][1]*a[2][2] ;
     det1 = a[0][0]*b[1]*a[2][2]+a[1][0]*b[2]*a[0][2]+a[2][0]*b[0]*a[1][2]-
            a[2][0]*b[1]*a[0][2]-a[0][0]*b[2]*a[1][2]-a[1][0]*b[0]*a[2][2] ;
     det2 = a[0][0]*a[1][1]*b[2]+a[1][0]*a[2][1]*b[0]+a[2][0]*a[0][1]*b[1]-
            a[2][0]*a[1][1]*b[0]-a[0][0]*a[2][1]*b[1]-a[1][0]*a[0][1]*b[2] ;
     c[0] = det0/det ;
     c[1] = det1/det ;
     c[2] = det2/det ;
    }

    return 1 ;
}

static int Msh3DTmp8( Msh3DBdryFace face, int node_indx )
{
    double             *c0, *c1, *c2, *c3 ;
    double             plane1_r_x_s[3], plane1_point[3], plane1_dplane ;
    double             plane2_r_x_s[3], plane2_point[3], plane2_dplane ;
    double             plane3_r_x_s[3], plane3_point[3], plane3_dplane ;
    double             a1_v1[3], a1_v2[3], a1_v3[3], a1_biss[3], a1_mod ;
    double             a2_v1[3], a2_v2[3], a2_v3[3], a2_biss[3], a2_mod ;
    double             cand_vec[3], r_x_s[3], den, dot, proj_vec[3], proj[3] ;
    double             a[3][3], b[3], c[3] ;
    double             center[3], rcirc, rinsc, ratio, tol ;

    char               m[80];

    /* evaluate the aspect ratio of element formed by the face and node_indx
       to avoid bad elements */

    tol = face->area * tol_factor ;

    c0 = node_list[node_indx].coord ;
    c1 = node_list[face->verts[0]].coord ;
    c2 = node_list[face->verts[1]].coord ;
    c3 = node_list[face->verts[2]].coord ;

    /* compute the radius of circumscribed sphere of the element. In 2D model,
       the center of a ciscumscribed sphere is located in the intersection of
       the normals of the edges. Here, in 3D model, this is done computing the
       intersection between three planes normal of three faces of the element.
       The radius of this sphere is the distance from the center to any point
       of the element */

    plane1_r_x_s[0] = c1[0] - c0[0] ;
    plane1_r_x_s[1] = c1[1] - c0[1] ;
    plane1_r_x_s[2] = c1[2] - c0[2] ;
    plane1_point[0] = 0.5*(c1[0]+c0[0]) ;
    plane1_point[1] = 0.5*(c1[1]+c0[1]) ;
    plane1_point[2] = 0.5*(c1[2]+c0[2]) ;
    plane1_dplane = plane1_r_x_s[0] * plane1_point[0] +
		    plane1_r_x_s[1] * plane1_point[1] +
		    plane1_r_x_s[2] * plane1_point[2] ;

    plane2_r_x_s[0] = c2[0] - c0[0] ;
    plane2_r_x_s[1] = c2[1] - c0[1] ;
    plane2_r_x_s[2] = c2[2] - c0[2] ;
    plane2_point[0] = 0.5*(c2[0]+c0[0]) ;
    plane2_point[1] = 0.5*(c2[1]+c0[1]) ;
    plane2_point[2] = 0.5*(c2[2]+c0[2]) ;
    plane2_dplane = plane2_r_x_s[0] * plane2_point[0] +
		    plane2_r_x_s[1] * plane2_point[1] +
		    plane2_r_x_s[2] * plane2_point[2] ;

    plane3_r_x_s[0] = c3[0] - c0[0] ;
    plane3_r_x_s[1] = c3[1] - c0[1] ;
    plane3_r_x_s[2] = c3[2] - c0[2] ;
    plane3_point[0] = 0.5*(c3[0]+c0[0]) ;
    plane3_point[1] = 0.5*(c3[1]+c0[1]) ;
    plane3_point[2] = 0.5*(c3[2]+c0[2]) ;
    plane3_dplane = plane3_r_x_s[0] * plane3_point[0] +
		    plane3_r_x_s[1] * plane3_point[1] +
		    plane3_r_x_s[2] * plane3_point[2] ;

    a[0][0] = plane1_r_x_s[0] ;
    a[0][1] = plane1_r_x_s[1] ;
    a[0][2] = plane1_r_x_s[2] ;
    a[1][0] = plane2_r_x_s[0] ;
    a[1][1] = plane2_r_x_s[1] ;
    a[1][2] = plane2_r_x_s[2] ;
    a[2][0] = plane3_r_x_s[0] ;
    a[2][1] = plane3_r_x_s[1] ;
    a[2][2] = plane3_r_x_s[2] ;
    b[0] = plane1_dplane ;
    b[1] = plane2_dplane ;
    b[2] = plane3_dplane ;
    if ( !Msh3DTmp8Cramer( 3, a, b, c ) ) return 1 ;
    center[0] = c[0] ; center[1] = c[1] ; center[2] = c[2] ;
    rcirc = sqrt( (center[0]-c0[0]) * (center[0]-c0[0]) +
                  (center[1]-c0[1]) * (center[1]-c0[1]) +
                  (center[2]-c0[2]) * (center[2]-c0[2]) ) ;

    /* compute the radius of inscribed sphere of the element. The center
       of a inscribed sphere is located in the intersection of the
       bisector of the angles of the element. Here this is done computing
       the intersection of two edges in the bisector of two angles of the
       element. The radius of this sphere is the distance from this center
       from the projection of this center into any face of the element */

    a1_v1[0] = c2[0]-c0[0] ; a1_v1[1] = c2[1]-c0[1] ; a1_v1[2] = c2[2]-c0[2] ;
    a1_v2[0] = c1[0]-c0[0] ; a1_v2[1] = c1[1]-c0[1] ; a1_v2[2] = c1[2]-c0[2] ;
    a1_v3[0] = c3[0]-c0[0] ; a1_v3[1] = c3[1]-c0[1] ; a1_v3[2] = c3[2]-c0[2] ;
    a1_biss[0] = a1_v1[0] + a1_v2[0] + a1_v3[0] ;
    a1_biss[1] = a1_v1[1] + a1_v2[1] + a1_v3[1] ;
    a1_biss[2] = a1_v1[2] + a1_v2[2] + a1_v3[2] ;

    a2_v1[0] = c2[0]-c1[0] ; a2_v1[1] = c2[1]-c1[1] ; a2_v1[2] = c2[2]-c1[2] ;
    a2_v2[0] = c0[0]-c1[0] ; a2_v2[1] = c0[1]-c1[1] ; a2_v2[2] = c0[2]-c1[2] ;
    a2_v3[0] = c3[0]-c1[0] ; a2_v3[1] = c3[1]-c1[1] ; a2_v3[2] = c3[2]-c1[2] ;
    a2_biss[0] = a2_v1[0] + a2_v2[0] + a2_v3[0] ;
    a2_biss[1] = a2_v1[1] + a2_v2[1] + a2_v3[1] ;
    a2_biss[2] = a2_v1[2] + a2_v2[2] + a2_v3[2] ;

    a[0][0] = a1_biss[0] ;
    a[1][0] = a1_biss[1] ;
    a[0][1] = -1.0 * a2_biss[0] ;
    a[1][1] = -1.0 * a2_biss[1] ;
    b[0] = c1[0] - c0[0] ;
    b[1] = c1[1] - c0[1] ;
    if ( !Msh3DTmp8Cramer( 2, a, b, c ) ) return 1 ;
    if ( !ABOUT_ZERO((a1_biss[2]*c[0]-a2_biss[2]*c[1])-(c1[2]-c0[2]),tol) )
     return 1 ;
    center[0] = c0[0] + c[0]*a1_biss[0] ;
    center[1] = c0[1] + c[0]*a1_biss[1] ;
    center[2] = c0[2] + c[0]*a1_biss[2] ;
    cand_vec[0] = center[0] - face->center[0] ;
    cand_vec[1] = center[1] - face->center[1] ;
    cand_vec[2] = center[2] - face->center[2] ;
    den = face->r_x_s[0] * face->r_x_s[0] +
	  face->r_x_s[1] * face->r_x_s[1] +
          face->r_x_s[2] * face->r_x_s[2] ;
    r_x_s[0] = face->r_x_s[0] / sqrt(den) ;
    r_x_s[1] = face->r_x_s[1] / sqrt(den) ;
    r_x_s[2] = face->r_x_s[2] / sqrt(den) ;
    dot = cand_vec[0] * r_x_s[0] +
	  cand_vec[1] * r_x_s[1] +
	  cand_vec[2] * r_x_s[2] ;
    r_x_s[0] *= dot ;
    r_x_s[1] *= dot ;
    r_x_s[2] *= dot ;
    proj_vec[0] = cand_vec[0] - r_x_s[0] ;
    proj_vec[1] = cand_vec[1] - r_x_s[1] ;
    proj_vec[2] = cand_vec[2] - r_x_s[2] ;
    proj[0] = proj_vec[0] + face->center[0] ;
    proj[1] = proj_vec[1] + face->center[1] ;
    proj[2] = proj_vec[2] + face->center[2] ;
    rinsc = sqrt( (center[0]-proj[0]) * (center[0]-proj[0]) +
                  (center[1]-proj[1]) * (center[1]-proj[1]) +
                  (center[2]-proj[2]) * (center[2]-proj[2]) ) ;

    /* return good element */

    ratio = (1.0/3.0) * (rcirc/rinsc) ;
    if ( ratio > 3.0 )
     return 0 ;

    return 1 ;
}

static int Msh3DTmp9(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                node_bdry )
{
    int                v1i, v2i, v1, v2, i, j, found, test, new = 0 ;
    Msh3DAdjFace       cur ;
    double             ind_vol = 0.0 ;
    double             ind_rms = 0.0 ;
    double             ind_sha = 0.0 ;

    /* test quality for faces in the boundary that chosen a node in the
       boundary, to avoid flat elements in that region. */

    if( face->use != 0 ) return 1 ;

    if( (face->verts[0] < node_bdry) &&
        (face->verts[1] < node_bdry) &&
        (face->verts[2] < node_bdry) &&
        (node_indx      < node_bdry)  )
     test = 1 ;
    else
     test = 0 ;
    if( !test ) return 1 ;
    else        return 0 ;

    /* compute the volume of tetrahedron */

    ind_vol = Msh3DVolum( face, node_list, node_indx ) ;

    /* compute the root_mean_square of tetrahedron */

    ind_rms = Msh3DSrms( face, node_list, node_indx ) ;

    /* compute shape of tetrahedron */

    ind_sha = (ind_rms * ind_rms * ind_rms) / (ind_vol) ;

    /* The node only will be choosen if it's between two limits */

    if( 1 ) {
     if( (ind_sha > (SHA_FACTOR / (MUL_FACTOR/25.0))) &&
         (ind_sha < (SHA_FACTOR * (MUL_FACTOR/25.0))) ) {
      return 1 ;
     }
     else {
      return 0 ;
     }
    }
    else {
     return 1 ;
    }
}

static int Msh3DTmp10(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int    i, j ;
    int    v1, v2, v3, v[3][3] ;
    double r[3], s[3], r_x_s[3], area ;
    double tol = 1e-09 ;

    /* Set tolerance */
#if OLD_TOLE
    tol = 1e-09 ;
#else
    tol = 1e-12 ;
#endif

    /* test if any of the three new faces will have a null area */

    v[0][0] = node_indx ;
    v[0][1] = face->verts[0] ;
    v[0][2] = face->verts[1] ;
    v[1][0] = node_indx ;
    v[1][1] = face->verts[0] ;
    v[1][2] = face->verts[2] ;
    v[2][0] = node_indx ;
    v[2][1] = face->verts[1] ;
    v[2][2] = face->verts[2] ;

    for (i = 0; i < 3; i++)
    {
     v1 = v[i][0] ;
     v2 = v[i][1] ;
     v3 = v[i][2] ;
     for (j = 0; j < 3; j++)
     {
      r[j] = node_list[v2].coord[j] - node_list[v1].coord[j] ;
      s[j] = node_list[v3].coord[j] - node_list[v1].coord[j] ;
     }
     r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
     r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
     r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
     area = sqrt( r_x_s[0] * r_x_s[0] +
		  r_x_s[1] * r_x_s[1] +
		  r_x_s[2] * r_x_s[2] ) / 2.0 ;
     if( ABOUT_ZERO(area,tol) )
      return 0 ;
    }

    /* if we get here, all faces have area bigger than zero */

    return 1 ;
}

static int Msh3DCheckInbox(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    double x, y, z ;

    /* Check if the new node is inside the octree box, otherwise
     * we're trying to mesh the outwards and it will never converge */

#if OLD_INSI
    return 1 ;
#else
    x = node_list[node_indx].coord[0] ;
    y = node_list[node_indx].coord[1] ;
    z = node_list[node_indx].coord[2] ;
    if ((x >= gmin[0]) && (x <= gmax[0]) &&
        (y >= gmin[1]) && (y <= gmax[1]) &&
        (z >= gmin[2]) && (z <= gmax[2])) {
     return 1 ;
    }
    else {
#if DIA_INSI
     fprintf (stderr, "Face created a node outside octree box:\n") ;
     fprintf (stderr, "Face (internally) = %d %d %d\n", face->verts[0],
		                                        face->verts[1],
			     		                face->verts[2]) ;
     fprintf (stderr, "Face (actually)   = %d %d %d\n",
		                   node_list[face->verts[0]].real_id,
				   node_list[face->verts[1]].real_id,
				   node_list[face->verts[2]].real_id) ;
     fprintf (stderr, "Node (internally) = %d\n", node_indx) ;
     fprintf (stderr, "Node (actually)   = %d\n",
		                   node_list[node_indx].real_id) ;
#endif
     return 0 ;
    }
#endif
}

/*
** ----------------------------------------------------------------------
** Geo routines - These routines check if two faces, defined by their
**                nodes (a,b,c - first face and u,v,w - second face)
**                intersect each other. This package is formed by the
**                following routines:
**                      1) GeoTriIntersect ;
**                      2) GeoTriSegIntersect ;
**                      3) GeoTriSegIntersectCheck ;
**                      4) GeoSegSegIntersect ;
**                      5) GeoNorm ;
**                      6) GeoCrossProd ;
**                      7) GeoCross ;
**                      8) GeoCrossNorm ;
**                      9) GeoDot.
*/

static int  GeoTriIntersect( int dir,
		             double a[3], double b[3], double c[3], int p[3],
		             double u[3], double v[3], double w[3], int q[3] )
{
    int                type ;

    /* test intersection against two given triangles and return if
       intersection that it's not coincident with any vertices is
       found */

    if (q[0]==0) {
     type = GeoTriSegIntersect(dir,a,b,c,u,v) ;
     if (type == 0)  return type ;
    }
    if (q[1]==0) {
     type = GeoTriSegIntersect(dir,a,b,c,v,w);
     if (type == 0)  return type ;
    }
    if (q[2]==0) {
     type = GeoTriSegIntersect(dir,a,b,c,w,u);
     if (type == 0)  return type ;
    }
    if (p[0]==0) {
     type = GeoTriSegIntersect(-dir,u,v,w,a,b);
     if (type == 0)  return type ;
    }
    if (p[1]==0) {
     type = GeoTriSegIntersect(-dir,u,v,w,b,c);
     if (type == 0)  return type ;
    }
    if (p[2]==0) {
     type = GeoTriSegIntersect(-dir,u,v,w,c,a);
     if (type == 0)  return type ;
    }

   /* if we get here no intersection was found */

   return 1 ;
}

static int GeoTriSegIntersect(
    int                dir,
    double             a[3],
    double             b[3],
    double             c[3],
    double             u[3],
    double             v[3] )
{
    int    inter ;
    double d, nc;
    double v1, v2;
    double p[3], n[3];
    double nt[3];          /* auxiliar vector to store normal */

    /* compute triangle normal vector */

    GeoCross(a,b,c,n);
    GeoNorm(n);

    /* compute triangle plane independent term */

    d = -a[0]*n[0] - a[1]*n[1] - a[2]*n[2];

    /* compute distances of segment endpoints to the triangle plane */

    v1 = u[0]*n[0] + u[1]*n[1] + u[2]*n[2] + d;
    v2 = v[0]*n[0] + v[1]*n[1] + v[2]*n[2] + d;

    /* check if triangle and segment are coplanar */

    if (fabs(v1) < tol_inters && fabs(v2) < tol_inters)
    {
     /* check if endpoints are inside triangle */

     if (GeoDot(n,GeoCrossNorm(u,a,b,nt)) > 0.0 &&
         GeoDot(n,GeoCrossNorm(u,b,c,nt)) > 0.0 &&
         GeoDot(n,GeoCrossNorm(u,c,a,nt)) > 0.0
        ) {
      inter = 1 ;   /* intersection point is u */
      if (GeoTriSegIntersectCheck (u,a)) inter = 0 ;
      if (GeoTriSegIntersectCheck (u,b)) inter = 0 ;
      if (GeoTriSegIntersectCheck (u,c)) inter = 0 ;
      if (inter) return 0 ;
     }
     if (GeoDot(n,GeoCrossNorm(v,a,b,nt)) > 0.0 &&
         GeoDot(n,GeoCrossNorm(v,b,c,nt)) > 0.0 &&
         GeoDot(n,GeoCrossNorm(v,c,a,nt)) > 0.0
        ) {
      inter = 1 ;   /* intersection point is v */
      if (GeoTriSegIntersectCheck (v,a)) inter = 0 ;
      if (GeoTriSegIntersectCheck (v,b)) inter = 0 ;
      if (GeoTriSegIntersectCheck (v,c)) inter = 0 ;
      if (inter) return 0 ;
     }

     if (GeoSegSegIntersect(a,b,u,v,p))
     {
      inter = 1 ;   /* intersection point is p */
      if (GeoTriSegIntersectCheck (p,a)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,b)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,c)) inter = 0 ;
      if (inter) return 0 ;
     }
     if (GeoSegSegIntersect(b,c,u,v,p))
     {
      inter = 1 ;   /* intersection point is p */
      if (GeoTriSegIntersectCheck (p,a)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,b)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,c)) inter = 0 ;
      if (inter) return 0 ;
     }
     if (GeoSegSegIntersect(c,a,u,v,p))
     {
      inter = 1 ;   /* intersection point is p */
      if (GeoTriSegIntersectCheck (p,a)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,b)) inter = 0 ;
      if (GeoTriSegIntersectCheck (p,c)) inter = 0 ;
      if (inter) return 0 ;
     }
    }

    /* check if segment cross or touch the triangle plane */

    else if (v1*v2 < 0.0)
    {
     /* compute intersection between segment and triangle */

     double t = v1/(v1-v2);
     p[0] = u[0] + t*(v[0]-u[0]);
     p[1] = u[1] + t*(v[1]-u[1]);
     p[2] = u[2] + t*(v[2]-u[2]);

     /* check if intersection point is inside triangle */

     if (GeoDot(n,GeoCrossNorm(p,a,b,nt)) >= -tol_inters &&
         GeoDot(n,GeoCrossNorm(p,b,c,nt)) >= -tol_inters &&
         GeoDot(n,GeoCrossNorm(p,c,a,nt)) >= -tol_inters
        )
     {
      {
       inter = 1 ;   /* intersection point is p */
       if (GeoTriSegIntersectCheck (p,a)) inter = 0 ;
       if (GeoTriSegIntersectCheck (p,b)) inter = 0 ;
       if (GeoTriSegIntersectCheck (p,c)) inter = 0 ;
       if (inter) return 0 ;
      }
     }
    }

    /* if we get here no intersection was found */

    return 1 ;
}

static int GeoTriSegIntersectCheck( double p[3], double v[3] )
{
    /* verify if the intersection point and the vertex are the same */

    if( ABOUT_ZERO((p[0]-v[0]),tol_inters) &&
        ABOUT_ZERO((p[1]-v[1]),tol_inters) &&
        ABOUT_ZERO((p[2]-v[2]),tol_inters)  )
     return 1 ;
    else
     return 0 ;
}

static int GeoSegSegIntersect
(double p1[3], double q1[3], double p2[3], double q2[3], double p[3])
{
 double det, t, l1, l2, c2, c[3];
 double v1[3], v2[3] ;

 v1[0] = q1[0]-p1[0];
 v1[1] = q1[1]-p1[1];
 v1[2] = q1[2]-p1[2];
 v2[0] = q2[0]-p2[0];
 v2[1] = q2[1]-p2[1];
 v2[2] = q2[2]-p2[2];
 l1 = GeoNorm(v1);
 l2 = GeoNorm(v2);
 c[0] = v1[1]*v2[2] - v1[2]*v2[1];
 c[1] = v1[2]*v2[0] - v1[0]*v2[2];
 c[2] = v1[0]*v2[1] - v1[1]*v2[0];
 c2 = (c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);

 /* test if segments are colinear */
 if (c2 < tol_inters)
  return 0;

 /* segments are not colinear */

 /* compute parameter related to second line */
 det = ((p2[0]-p1[0])*v1[1]*c[2] +
        (p2[1]-p1[1])*v1[2]*c[0] +
        (p2[2]-p1[2])*v1[0]*c[1]
       )
       -
       (
        (p2[2]-p1[2])*v1[1]*c[0] +
        (p2[0]-p1[0])*v1[2]*c[1] +
        (p2[1]-p1[1])*v1[0]*c[2]
       );
 t = det/c2;
 if (t < 0.0 || t > l2)
  return 0;

 /* compute parameter related to first line */
 det = ((p2[0]-p1[0])*v2[1]*c[2] +
        (p2[1]-p1[1])*v2[2]*c[0] +
        (p2[2]-p1[2])*v2[0]*c[1]
       )
       -
       (
        (p2[2]-p1[2])*v2[1]*c[0] +
        (p2[0]-p1[0])*v2[2]*c[1] +
        (p2[1]-p1[1])*v2[0]*c[2]
       );
 t = det/c2;
 if (t < 0.0 || t > l1)
  return 0;
 else
 {
  p[0] = p1[0] + t*v1[0];
  p[1] = p1[1] + t*v1[1];
  p[2] = p1[2] + t*v1[2];
  return 1;
 }
}

static double GeoNorm (double v[3])
{
 double lenght = (double)(sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
#if OLD_NORM
 if( !ABOUT_ZERO(lenght,1.0e-7) ) {
  v[0] /= lenght; v[1] /= lenght; v[2] /= lenght;
 }
#else
 if (jmesh_version == SMALL_VERSION) {
  if( !ABOUT_ZERO(lenght,1.0e-7) ) {
   v[0] /= lenght; v[1] /= lenght; v[2] /= lenght;
  }
 } else {
  if( !ABOUT_ZERO(lenght,1.0e-12) ) {
   v[0] /= lenght; v[1] /= lenght; v[2] /= lenght;
  }
 }
#endif
 return lenght;
}

static double *GeoCrossProd(double a[3], double b[3], double n[3])
{
 n[0] = (a[1]*b[2]) - (a[2]*b[1]) ;
 n[1] = (a[2]*b[0]) - (a[0]*b[2]) ;
 n[2] = (a[0]*b[1]) - (a[1]*b[0]) ;
 return n;
}

static double *GeoCross (double a[3], double b[3], double c[3], double n[3])
{
 n[0] = (b[1]-a[1])*(c[2]-a[2]) - (b[2]-a[2])*(c[1]-a[1]);
 n[1] = (b[2]-a[2])*(c[0]-a[0]) - (b[0]-a[0])*(c[2]-a[2]);
 n[2] = (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
 return n;
}

static double *GeoCrossNorm (double a[3], double b[3], double c[3], double n[3])
{
 n[0] = (b[1]-a[1])*(c[2]-a[2]) - (b[2]-a[2])*(c[1]-a[1]);
 n[1] = (b[2]-a[2])*(c[0]-a[0]) - (b[0]-a[0])*(c[2]-a[2]);
 n[2] = (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
 GeoNorm (n);
 return n;
}

static double GeoDot (double u[3], double v[3])
{
 return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

/*
** ----------------------------------------------------------------------
** Intersect routines - These routines check if two faces, defined
**                      by nface and fface, intersect each other.
**                      This package is formed by the following routines:
**                      1) FaceIntersectCoPlanar ;
**                      2) FaceIntersect ;
**                      3) CheckHasElement ;
**                      4) CheckInPlaneIntsct ;
**                      5) IntersectFaceWithPlane ;
**                      6) ThreePlaneIntersect ;
**                      7) LineFaceIntersect ;
**                      8) CheckCross ;
**                      9) CrossProd ;
**                     10) TripleCrossProd.
*/

/* -------------------------------------------------------------
**
**     FaceIntersectCoPlanar - check for face intersection
**
**       int FaceIntersectCoPlanar(
**         nface_coords[3][3],
**         nface_nodeid[3],
**         fface_coords[3][3],
**         fface_nodeid[3])
**
**         nface_coords - (in) new face coordinates
**         nface_nodeid - (in) new face nodes (ids)
**         fface_coords - (in) current face coordinates
**         fface_nodeid - (in) current face nodes (ids)
**
**       Description: Checks to see if two co-planar faces intersect.
**
**       Return Value: true if they intersect
**
**
**  -- */

static int FaceIntersectCoPlanar(
    double  nface_coords[3][3], /* node coords of a possible new face */
    int     nface_nodeid[3],    /* node ids    of a possible new face */
    double  fface_coords[3][3], /* node coords of a current face */
    int     fface_nodeid[3] )   /* node ids    of a current face */
{
    int i, j, k, dir ;
    int  num_com = 0 ;
    int  common[3] = {-1,-1,-1} ;
    int  invcom[3] = {-1,-1,-1} ;
    double cross0[3],cross1[3] ;
    double vec[3], dif[3] ;
    double nedge0[3],nedge1[3],fedge0[3],fedge1[3] ;
    double nf2d[3][2], ff2d[3][2] ;
    double len, norm[3] ;
    double prod0, prod1 ;

    /* this routine checks to see if two non-coplanar faces intersect */

    /* compute normal of new face */

    TripleCrossProd (nface_coords[0],nface_coords[1],nface_coords[2],norm) ;
    len = sqrt (norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]) ;
    if (fabs(len) >= 1e-12) {
     norm[0] /= len ;
     norm[1] /= len ;
     norm[2] /= len ;
    }

    /* the first thing we need to do is to determine which
     * nodes, if any, that the two faces share */

    if ((nface_nodeid[0] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[0]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[0], fface_nodeid[0])))) {
        common[0] = 0 ;  invcom[0] = 0 ;  ++num_com ;
    } else if ((nface_nodeid[0] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[0]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[0], fface_nodeid[1])))) {
        common[0] = 1 ;  invcom[1] = 0 ;  ++num_com ;
    } else if ((nface_nodeid[0] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[0]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[0], fface_nodeid[2])))) {
        common[0] = 2 ;  invcom[2] = 0 ;  ++num_com ;
    }

    if ((nface_nodeid[1] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[1]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[1], fface_nodeid[0])))) {
        common[1] = 0 ;  invcom[0] = 1 ;  ++num_com ;
    } else if ((nface_nodeid[1] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[1]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[1], fface_nodeid[1])))) {
        common[1] = 1 ;  invcom[1] = 1 ;  ++num_com ;
    } else if ((nface_nodeid[1] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[1]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[1], fface_nodeid[2])))) {
        common[1] = 2 ;  invcom[2] = 1 ;  ++num_com ;
    }

    if ((nface_nodeid[2] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[2]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[2], fface_nodeid[0])))) {
        common[2] = 0 ;  invcom[0] = 2 ;  ++num_com ;
    } else if ((nface_nodeid[2] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[2]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[2], fface_nodeid[1])))) {
        common[2] = 1 ;  invcom[1] = 2 ;  ++num_com ;
    } else if ((nface_nodeid[2] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[2]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[2], fface_nodeid[2])))) {
        common[2] = 2 ;  invcom[2] = 2 ;  ++num_com ;
    }

    if (num_com == 3) return(false) ;

    /* if the faces share two nodes then we need to check to
     * make sure that the two outstanding nodes are on opposite
     * sided of the edge common to the two faces. */

    if (num_com == 2) {
        for (i=0 ; i<3 ; ++i) {
            j = (i+1) % 3 ;
            if ((common[i] >= 0) && common[j] >= 0) {
		vec[0] = nface_coords[j][0] - nface_coords[i][0] ;
		vec[1] = nface_coords[j][1] - nface_coords[i][1] ;
		vec[2] = nface_coords[j][2] - nface_coords[i][2] ;
		dif[0] = nface_coords[(i+2)%3][0] - nface_coords[i][0] ;
		dif[1] = nface_coords[(i+2)%3][1] - nface_coords[i][1] ;
		dif[2] = nface_coords[(i+2)%3][2] - nface_coords[i][2] ;
		CrossProd (vec, dif, cross0) ;
                for (k=0 ; k<3 ; ++k) {
                    if (invcom[k] == -1) break ;
                }
		dif[0] = fface_coords[k][0] - nface_coords[i][0] ;
		dif[1] = fface_coords[k][1] - nface_coords[i][1] ;
		dif[2] = fface_coords[k][2] - nface_coords[i][2] ;
		CrossProd (vec, dif, cross1) ;
                break ;
            }
        }
	prod0 = (cross0[0]*norm[0] + cross0[1]*norm[1] + cross0[2]*norm[2]) ;
	prod1 = (cross1[0]*norm[0] + cross1[1]*norm[1] + cross1[2]*norm[2]) ;
        return((prod0*prod1) > 0.0) ;
    }

    /* if the two faces share one node then we need to see if
     * either of the edges of the front face connected to the
     * common node falls between the edges on the newface connected
     * to the common node */

    if (num_com == 1) {
        for (i=0 ; i<3 ; ++i) {
            if (common[i] >= 0) {
                nedge0[0] = nface_coords[(i+1)%3][0] - nface_coords[i][0] ;
                nedge0[1] = nface_coords[(i+1)%3][1] - nface_coords[i][1] ;
                nedge0[2] = nface_coords[(i+1)%3][2] - nface_coords[i][2] ;
                nedge1[0] = nface_coords[(i+2)%3][0] - nface_coords[i][0] ;
                nedge1[1] = nface_coords[(i+2)%3][1] - nface_coords[i][1] ;
                nedge1[2] = nface_coords[(i+2)%3][2] - nface_coords[i][2] ;
                for (k=0 ; k<3 ; ++k) {
                    if (invcom[k] >= 0) break ;
                }
                fedge0[0] = nface_coords[(k+1)%3][0] - nface_coords[i][0] ;
                fedge0[1] = nface_coords[(k+1)%3][1] - nface_coords[i][1] ;
                fedge0[2] = nface_coords[(k+1)%3][2] - nface_coords[i][2] ;
                fedge1[0] = nface_coords[(k+2)%3][0] - nface_coords[i][0] ;
                fedge1[1] = nface_coords[(k+2)%3][1] - nface_coords[i][1] ;
                fedge1[2] = nface_coords[(k+2)%3][2] - nface_coords[i][2] ;
                break ;
            }
        }
	CrossProd (nedge0, fedge0, cross0) ;
	CrossProd (nedge1, fedge0, cross1) ;
	prod0 = (cross0[0]*norm[0] + cross0[1]*norm[1] + cross0[2]*norm[2]) ;
	prod1 = (cross1[0]*norm[0] + cross1[1]*norm[1] + cross1[2]*norm[2]) ;
	if ((prod0 * prod1) < 0.0) return (1) ;
	CrossProd (nedge0, fedge1, cross0) ;
	CrossProd (nedge1, fedge1, cross1) ;
	prod0 = (cross0[0]*norm[0] + cross0[1]*norm[1] + cross0[2]*norm[2]) ;
	prod1 = (cross1[0]*norm[0] + cross1[1]*norm[1] + cross1[2]*norm[2]) ;
	if ((prod0 * prod1) < 0.0) return (1) ;
        return(0) ;
    }

    /*  the two faces do not share any nodes so we need to check to
     *  see if any of the edges of one triangle intersects any of
     *  the edges of the other */

    /* to make these check we project everything on to one of the
     *  three coordinate axis planes */

    if (norm[0] > norm[1]) {  /* normal.x > normal.y */
        dir = (norm[0] > norm[2]) ? 0 : 2 ;
    } else {
        dir = (norm[1] > norm[2]) ? 1 : 2 ;
    }

    switch (dir) {
        case 0:
            for (i=0 ; i<3 ; ++i) {
		nf2d[i][0] = nface_coords[i][1] ;
		nf2d[i][1] = nface_coords[i][2] ;
		ff2d[i][0] = fface_coords[i][1] ;
		ff2d[i][1] = fface_coords[i][2] ;
            }
            break ;
        case 1:
            for (i=0 ; i<3 ; ++i) {
		nf2d[i][0] = nface_coords[i][2] ;
		nf2d[i][1] = nface_coords[i][0] ;
		ff2d[i][0] = fface_coords[i][2] ;
		ff2d[i][1] = fface_coords[i][0] ;
            }
            break ;
        case 2:
            for (i=0 ; i<3 ; ++i) {
		nf2d[i][0] = nface_coords[i][0] ;
		nf2d[i][1] = nface_coords[i][1] ;
		ff2d[i][0] = fface_coords[i][0] ;
		ff2d[i][1] = fface_coords[i][1] ;
            }
            break ;
    }

    /* check to see if the edges cross */

    for (i=0 ; i<3 ; ++i) {
        j = (i+1) % 3 ;
        for (k=0 ; k<3 ; ++k) {
            if (CheckCross(nf2d[i],nf2d[j],ff2d[k],ff2d[(k+1)%3]))
                return(1) ;
        }
    }
    return(0) ;
}

/* -------------------------------------------------------------
**
**     FaceIntersect - check for face intersection
**
**       int FaceIntersect(
**         nface_coords[3][3],
**         nface_nodeid[3],
**         fface_coords[3][3],
**         fface_nodeid[3])
**
**         nface_coords - (in) new face coordinates
**         nface_nodeid - (in) new face nodes (ids)
**         fface_coords - (in) current face coordinates
**         fface_nodeid - (in) current face nodes (ids)
**
**       Description: Checks to see if two faces intersect.
**
**       Return Value: true if they intersect
**
**
**  -- */

static int FaceIntersect(
    double  nface_coords[3][3], /* node coords of a possible new face */
    int     nface_nodeid[3],    /* node ids    of a possible new face */
    double  fface_coords[3][3], /* node coords of a current face */
    int     fface_nodeid[3])    /* node ids    of a current face */
{
    int i, j, k, dir ;
    int  num_com = 0 ;
    int  common[3] = {-1,-1,-1} ;
    int  invcom[3] = {-1,-1,-1} ;
    int sign[3], isign[3], zeros0 = 0, zeros1 = 0 ;
    double cross0[3], cross1[3], ignore ;
    double vec[3], dif[3], value, mag, prod ;
    double nedge0[3],nedge1[3],fedge0[3],fedge1[3] ;
    double nf2d[3][2], ff2d[3][2] ;
    double r[3], s[3], r_x_s[3] ;
    double prod0, prod1, len, vect[3], diff[3] ;
    double cn[2], cf[2] ;
    double tol0, tol1, tol ;
    double fnorm[3], fcent[3], farea, ftol, fd ;
    double nnorm[3], ncent[3], narea, ntol, nd ;
    double nintsct[2][3], fintsct[2][3] ;

    /* this routine checks to see if two non-coplanar faces intersect */

    /* compute informations for both faces, such as normals, etc... */

    /* fface normal */
    TripleCrossProd (fface_coords[0],fface_coords[1],fface_coords[2],fnorm) ;
    len = sqrt (fnorm[0]*fnorm[0]+fnorm[1]*fnorm[1]+fnorm[2]*fnorm[2]) ;
    if (fabs(len) >= 1e-12) {
     fnorm[0] /= len ;
     fnorm[1] /= len ;
     fnorm[2] /= len ;
    }
    /* fface centroid */
    for (i = 0; i < 3; i++) {
     fcent[i] = 0.0 ;
     for (j = 0; j < 3; j++) fcent[i] += fface_coords[j][i] ;
     fcent[i] /= 3.0 ;
    }
    /* fface tolerance */
    for (i = 0; i < 3; i++) {
     r[i] = fface_coords[1][i] - fface_coords[0][i] ;
     s[i] = fface_coords[2][i] - fface_coords[0][i] ;
    }
    r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
    r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
    r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
    farea = sqrt( r_x_s[0] * r_x_s[0] +
 		  r_x_s[1] * r_x_s[1] +
		  r_x_s[2] * r_x_s[2] ) / 2.0 ;
    ftol = FACE_TOLERANCE * sqrt(farea) ;
    /* fface constant for plane equation */
    fd = fnorm[0]*fface_coords[0][0] +
	 fnorm[1]*fface_coords[0][1] +
	 fnorm[2]*fface_coords[0][2] ;
    /* nface normal */
    TripleCrossProd (nface_coords[0],nface_coords[1],nface_coords[2],nnorm) ;
    len = sqrt (nnorm[0]*nnorm[0]+nnorm[1]*nnorm[1]+nnorm[2]*nnorm[2]) ;
    if (fabs(len) >= 1e-12) {
     nnorm[0] /= len ;
     nnorm[1] /= len ;
     nnorm[2] /= len ;
    }
    /* nface centroid */
    for (i = 0; i < 3; i++) {
     ncent[i] = 0.0 ;
     for (j = 0; j < 3; j++) ncent[i] += nface_coords[j][i] ;
     ncent[i] /= 3.0 ;
    }
    /* nface tolerance */
    for (i = 0; i < 3; i++) {
     r[i] = nface_coords[1][i] - nface_coords[0][i] ;
     s[i] = nface_coords[2][i] - nface_coords[0][i] ;
    }
    r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
    r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
    r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
    narea = sqrt( r_x_s[0] * r_x_s[0] +
 		  r_x_s[1] * r_x_s[1] +
		  r_x_s[2] * r_x_s[2] ) / 2.0 ;
    ntol = FACE_TOLERANCE * sqrt(narea) ;
    /* nface constant for plane equation */
    nd = nnorm[0]*nface_coords[0][0] +
	 nnorm[1]*nface_coords[0][1] +
	 nnorm[2]*nface_coords[0][2] ;

    /* the first thing we need to do is to determine which
     * nodes, if any, that the two faces share. */

    if ((nface_nodeid[0] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[0]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[0], fface_nodeid[0])))) {
        common[0] = 0 ;  invcom[0] = 0 ;  ++num_com ;
    } else if ((nface_nodeid[0] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[0]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[0], fface_nodeid[1])))) {
        common[0] = 1 ;  invcom[1] = 0 ;  ++num_com ;
    } else if ((nface_nodeid[0] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[0]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[0], fface_nodeid[2])))) {
        common[0] = 2 ;  invcom[2] = 0 ;  ++num_com ;
    }

    if ((nface_nodeid[1] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[1]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[1], fface_nodeid[0])))) {
        common[1] = 0 ;  invcom[0] = 1 ;  ++num_com ;
    } else if ((nface_nodeid[1] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[1]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[1], fface_nodeid[1])))) {
        common[1] = 1 ;  invcom[1] = 1 ;  ++num_com ;
    } else if ((nface_nodeid[1] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[1]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[1], fface_nodeid[2])))) {
        common[1] = 2 ;  invcom[2] = 1 ;  ++num_com ;
    }

    if ((nface_nodeid[2] == fface_nodeid[0]) ||
        ((node_list[nface_nodeid[2]].mates[0] != 0) &&
	 (CheckHasElement (nface_nodeid[2], fface_nodeid[0])))) {
        common[2] = 0 ;  invcom[0] = 2 ;  ++num_com ;
    } else if ((nface_nodeid[2] == fface_nodeid[1]) ||
               ((node_list[nface_nodeid[2]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[2], fface_nodeid[1])))) {
        common[2] = 1 ;  invcom[1] = 2 ;  ++num_com ;
    } else if ((nface_nodeid[2] == fface_nodeid[2]) ||
               ((node_list[nface_nodeid[2]].mates[0] != 0) &&
  	        (CheckHasElement (nface_nodeid[2], fface_nodeid[2])))) {
        common[2] = 2 ;  invcom[2] = 2 ;  ++num_com ;
    }

    /* check to see if the faces share two nodes.  If
     * this is the case they cannot intersect */

    if (num_com == 2) return(0) ;

    /* determine if the nodes of one triangle fall above, on,
     * or below the plane of the other triangle.  If the node
     * is joined to a common node then check based on the angle
     * (< about 1.15 degrees) otherwise check based on the face's
     * tolerance. */

    zeros0 = 0 ;
    zeros1 = 0 ;
    for (i = 0 ; i<3 ; ++i) {
        j = (i+1)%3 ;
        k = i==0 ? 2 : i-1 ;
        if (common[j] >= 0) {
            vect[0] = nface_coords[i][0] - nface_coords[j][0] ;
            vect[1] = nface_coords[i][1] - nface_coords[j][1] ;
            vect[2] = nface_coords[i][2] - nface_coords[j][2] ;
	    mag = sqrt (vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]) ;
	    prod = (vect[0]*fnorm[0] + vect[1]*fnorm[1] + vect[2]*fnorm[2]) ;
	    value = prod / mag ;
            if (value > 0.02) {
                sign[i] = 1 ;
            } else if (value < -0.02) {
                sign[i] = -1 ;
            } else {
                sign[i] = 0 ;
                ++zeros0 ;
            }
        } else if (common[k] >= 0) {
            vect[0] = nface_coords[i][0] - nface_coords[k][0] ;
            vect[1] = nface_coords[i][1] - nface_coords[k][1] ;
            vect[2] = nface_coords[i][2] - nface_coords[k][2] ;
	    mag = sqrt (vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]) ;
	    prod = (vect[0]*fnorm[0] + vect[1]*fnorm[1] + vect[2]*fnorm[2]) ;
	    value = prod / mag ;
            if (value > 0.02) {
                sign[i] = 1 ;
            } else if (value < -0.02) {
                sign[i] = -1 ;
            } else {
                sign[i] = 0 ;
                ++zeros0 ;
            }
        } else {
	    vect[0] = nface_coords[i][0] - fcent[0] ;
	    vect[1] = nface_coords[i][1] - fcent[1] ;
	    vect[2] = nface_coords[i][2] - fcent[2] ;
	    value = (vect[0]*fnorm[0] + vect[1]*fnorm[1] + vect[2]*fnorm[2]) ;
            if (value > ntol) {
                sign[i] = 1 ;
            } else if (value < -ntol) {
                sign[i] = -1 ;
            } else {
                sign[i] = 0 ;
                ++zeros0 ;
            }
        }
        if (invcom[j] >= 0) {
	    vect[0] = fface_coords[i][0] - fface_coords[j][0] ;
	    vect[1] = fface_coords[i][1] - fface_coords[j][1] ;
	    vect[2] = fface_coords[i][2] - fface_coords[j][2] ;
	    mag = sqrt (vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]) ;
	    prod = (vect[0]*fnorm[0] + vect[1]*fnorm[1] + vect[2]*fnorm[2]) ;
	    value = prod / mag ;
            if (value > 0.02) {
                isign[i] = 1 ;
            } else if (value < -0.02) {
                isign[i] = -1 ;
            } else {
                isign[i] = 0 ;
                ++zeros1 ;
            }
        } else if (invcom[k] >= 0) {
	    vect[0] = fface_coords[i][0] - fface_coords[k][0] ;
	    vect[1] = fface_coords[i][1] - fface_coords[k][1] ;
	    vect[2] = fface_coords[i][2] - fface_coords[k][2] ;
	    mag = sqrt (vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]) ;
	    prod = (vect[0]*fnorm[0] + vect[1]*fnorm[1] + vect[2]*fnorm[2]) ;
	    value = prod / mag ;
            if (value > 0.02) {
                isign[i] = 1 ;
            } else if (value < -0.02) {
                isign[i] = -1 ;
            } else {
                isign[i] = 0 ;
                ++zeros1 ;
            }
        } else {
	    vect[0] = fface_coords[i][0] - ncent[0] ;
	    vect[1] = fface_coords[i][1] - ncent[1] ;
	    vect[2] = fface_coords[i][2] - ncent[2] ;
	    value = (vect[0]*nnorm[0] + vect[1]*nnorm[1] + vect[2]*nnorm[2]) ;
            if (value > ftol) {
                isign[i] = 1 ;
            } else if (value < -ftol) {
                isign[i] = -1 ;
            } else {
                isign[i] = 0 ;
                ++zeros1 ;
            }
        }
    }
    if ((zeros0 >= 2) && (zeros1 >= 2)) return(0) ;

    /* now check to see if all the nodes of the new face are on
     * the same side of the front face or if all of the nodes
     * of the front face are on the same side of the new face.
     * If so, no intersect. */

    if ((sign[0]*sign[1] > 0)   && ((sign[0]*sign[2]) > 0))   return(0) ;
    if ((isign[0]*isign[1] > 0) && ((isign[0]*isign[2]) > 0)) return(0) ;

    if (num_com == 1) {

        /* if the faces share a node then if the other two nodes are
         * on the same side of the face they cannot intersect.  If
         * one of the adjacent nodes is in the plane of the face
         * then we need to check to see if it intersects an
         * edge of the face. */

        if (common[0] >= 0) {
            if (sign[1]*sign[2] > 0) {
                return(0) ;
            } else if (sign[1] == 0) {
                if (CheckInPlaneIntsct(nface_coords[0],
                                       nface_coords[1],
                                       fface_coords[(common[0]+1)%3],
                                       fface_coords[(common[0]+2)%3],
                                       fnorm)) return(1) ;
            } else if (sign[2] == 0) {
                if (CheckInPlaneIntsct(nface_coords[0],
                                       nface_coords[2],
                                       fface_coords[(common[0]+1)%3],
                                       fface_coords[(common[0]+2)%3],
                                       fnorm)) return(1) ;
            }
        } else if (common[1] >= 0) {
            if (sign[2]*sign[0] > 0) {
                return(0) ;
            } else if (sign[0] == 0) {
                if (CheckInPlaneIntsct(nface_coords[1],
                                       nface_coords[0],
                                       fface_coords[(common[1]+1)%3],
                                       fface_coords[(common[1]+2)%3],
                                       fnorm)) return(1) ;
            } else if (sign[2] == 0) {
                if (CheckInPlaneIntsct(nface_coords[1],
                                       nface_coords[2],
                                       fface_coords[(common[1]+1)%3],
                                       fface_coords[(common[1]+2)%3],
                                       fnorm)) return(1) ;
            }
        } else if (common[2] >= 0) {
            if (sign[0]*sign[1] > 0) {
                return(0) ;
            } else if (sign[0] == 0) {
                if (CheckInPlaneIntsct(nface_coords[2],
                                       nface_coords[0],
                                       fface_coords[(common[2]+1)%3],
                                       fface_coords[(common[2]+2)%3],
                                       fnorm)) return(1) ;
            } else if (sign[1] == 0) {
                if (CheckInPlaneIntsct(nface_coords[2],
                                       nface_coords[1],
                                       fface_coords[(common[2]+1)%3],
                                       fface_coords[(common[2]+2)%3],
                                       fnorm)) return(1) ;
            }
        }

        if (invcom[0] >= 0) {
            if (isign[1]*isign[2] > 0) {
                return(0) ;
            } else if (isign[1] == 0) {
                if (CheckInPlaneIntsct(fface_coords[0],
                                       fface_coords[1],
                                       nface_coords[(invcom[0]+1)%3],
                                       nface_coords[(invcom[0]+2)%3],
                                       nnorm)) return(1) ;
            } else if (isign[2] == 0) {
                if (CheckInPlaneIntsct(fface_coords[0],
                                       fface_coords[2],
                                       nface_coords[(invcom[0]+1)%3],
                                       nface_coords[(invcom[0]+2)%3],
                                       nnorm)) return(1) ;
            }
        } else if (invcom[1] >= 0) {
            if (isign[2]*isign[0] > 0) {
                return(0) ;
            } else if (isign[0] == 0) {
                if (CheckInPlaneIntsct(fface_coords[1],
                                       fface_coords[0],
                                       nface_coords[(invcom[1]+1)%3],
                                       nface_coords[(invcom[1]+2)%3],
                                       nnorm)) return(1) ;
            } else if (isign[2] == 0) {
                if (CheckInPlaneIntsct(fface_coords[1],
                                       fface_coords[2],
                                       nface_coords[(invcom[1]+1)%3],
                                       nface_coords[(invcom[1]+2)%3],
                                       nnorm)) return(1) ;
            }
        } else if (invcom[2] >= 0) {
            if (isign[0]*isign[1] > 0) {
                return(0) ;
            } else if (isign[0] == 0) {
                if (CheckInPlaneIntsct(fface_coords[2],
                                       fface_coords[0],
                                       nface_coords[(invcom[2]+1)%3],
                                       nface_coords[(invcom[2]+2)%3],
                                       nnorm)) return(1) ;
            } else if (isign[1] == 0) {
                if (CheckInPlaneIntsct(fface_coords[2],
                                       fface_coords[1],
                                       nface_coords[(invcom[2]+1)%3],
                                       nface_coords[(invcom[2]+2)%3],
                                       nnorm)) return(1) ;
            }
        }

        /* if the nodes straddle the face then check to see if the
         * opposite edge intersects the plane of the face inside
         * the face. */

        for (i=0 ; i<3 ; ++i) {
            if (common[i] >= 0) {
                if (LineFaceIntersect(nface_coords[(i+1)%3],
                                      nface_coords[(i+2)%3],
				      fface_coords[0],
				      fface_coords[1],
                                      fface_coords[2],
				      &ignore, 0)) return(1) ;
                break ;
            }
        }
        for (i=0 ; i<3 ; ++i) {
            if (invcom[i] >= 0) {
                if (LineFaceIntersect(fface_coords[(i+1)%3],
                                      fface_coords[(i+2)%3],
				      nface_coords[0],
				      nface_coords[1],
				      nface_coords[2],
				      &ignore, 0)) return(1) ;
                break ;
            }
        }
        return(0) ;
    } else {

        /* If the faces do not share a node, and one node is in
         * the plane of the face the the other two nodes must
         * straddle the face if they are to intersect. */

        if ((sign[0] == 0) && ((sign[1]*sign[2]) > 0)) return(0) ;
        if ((sign[1] == 0) && ((sign[2]*sign[0]) > 0)) return(0) ;
        if ((sign[2] == 0) && ((sign[0]*sign[1]) > 0)) return(0) ;
        if ((isign[0] == 0) && ((isign[1]*isign[2]) > 0)) return(0) ;
        if ((isign[1] == 0) && ((isign[2]*isign[0]) > 0)) return(0) ;
        if ((isign[2] == 0) && ((isign[0]*isign[1]) > 0)) return(0) ;
    }

    /* at this point we know that the two faces each have a part
     * on the line that is the intersection of the two planes.  We
     * need to determine if the parts of this line associated with
     * the triangles overlap or not. */

    IntersectFaceWithPlane(sign,nface_coords,nnorm,nd,fnorm,fd,nintsct) ;
    IntersectFaceWithPlane(isign,fface_coords,fnorm,fd,nnorm,nd,fintsct) ;

    /* determine the coordinate direction along which the
     * difference between the intersection points is greatest,
     * and check to see if the intersection segments overlap
     * in this direction. */

    diff[0] = fabs(nintsct[1][0] - nintsct[0][0]) ;
    diff[1] = fabs(nintsct[1][1] - nintsct[0][1]) ;
    diff[2] = fabs(nintsct[1][2] - nintsct[0][2]) ;
    if (diff[0] > diff[1]) {
        dir = (diff[0] > diff[2]) ? 0 : 2 ;
    } else {
        dir = (diff[1] > diff[2]) ? 1 : 2 ;
    }

    switch (dir) {
        case 0:
            if (nintsct[0][0] < nintsct[1][0]) {
                cn[0] = nintsct[0][0] ;
                cn[1] = nintsct[1][0] ;
            } else {
                cn[0] = nintsct[1][0] ;
                cn[1] = nintsct[0][0] ;
            }
            if (fintsct[0][0] < fintsct[1][0]) {
                cf[0] = fintsct[0][0] ;
                cf[1] = fintsct[1][0] ;
            } else {
                cf[0] = fintsct[1][0] ;
                cf[1] = fintsct[0][0] ;
            }
            break ;
        case 1:
            if (nintsct[0][1] < nintsct[1][1]) {
                cn[0] = nintsct[0][1] ;
                cn[1] = nintsct[1][1] ;
            } else {
                cn[0] = nintsct[1][1] ;
                cn[1] = nintsct[0][1] ;
            }
            if (fintsct[0][1] < fintsct[1][1]) {
                cf[0] = fintsct[0][1] ;
                cf[1] = fintsct[1][1] ;
            } else {
                cf[0] = fintsct[1][1] ;
                cf[1] = fintsct[0][1] ;
            }
            break ;
        case 2:
            if (nintsct[0][2] < nintsct[1][2]) {
                cn[0] = nintsct[0][2] ;
                cn[1] = nintsct[1][2] ;
            } else {
                cn[0] = nintsct[1][2] ;
                cn[1] = nintsct[0][2] ;
            }
            if (fintsct[0][2] < fintsct[1][2]) {
                cf[0] = fintsct[0][2] ;
                cf[1] = fintsct[1][2] ;
            } else {
                cf[0] = fintsct[1][2] ;
                cf[1] = fintsct[0][2] ;
            }
            break ;
    }

    /* do the check */

    tol0 = fabs(cn[0] - cn[1]) ;
    tol1 = fabs(cf[0] - cf[1]) ;
    tol = tol0 > tol1 ? tol0 : tol1 ;
    tol *= 1e-8 ;
    if ((cn[0] > cf[1]-tol) || (cn[1] < cf[0]+tol)) return(0) ;

    return(1) ;
}

/* ---------------------------------------------------------------------
** CheckHasElement - Check if mate list of one node has the other.
*/

int CheckHasElement (
    int  nnodeid,   /* Node where the search will be made */
    int  fnodeid )  /* Node to be searched in mate list   */
{
 int i ;

 /* If the node where the search will be made does not have mates,
  * return indicating that no element was found */

 if (node_list[nnodeid].mates[0] == 0) return 0 ;

 /* Otherwise check if node to be searched it is in the mate list */

 for (i = 1 ; i <= node_list[nnodeid].mates[0] ; i++) {
  if (node_list[nnodeid].mates[i] == fnodeid) return 1 ;
 }

 /* If we get here, the node to be searched it is not in mate list */

 return 0 ;
}

/* -------------------------------------------------------------
**
** CheckInPlaneIntsct - check line intersection
**
** int CheckInPlaneIntsct(double pt0[3],
**                        double pt1[3],
**                        double pt2[3],
**                        double pt3[3],
**                        double normal[3])
**
**        pt0    - (in)  first coordinate
**        pt1    - (in)  second coordinate
**        pt2    - (in)  third coordinate
**        pt3    - (in)  fourth coordinate
**        normal - (in)  plane normal
**
**      Description: This routine takes four points in the same and
**          checks to see if the line between the first two points
**          crosses the line between the second two.
**
**      Return Value: true if they intersect
**
**
** -- */

int CheckInPlaneIntsct(double pt0[3],
                       double pt1[3],
                       double pt2[3],
                       double pt3[3],
                       double normal[3])
{
    double vect1[3], vect2[3], vect3[3] ;
    double t2, t3, t, cross[3] ;
    int    i, result ;

    /* This routine takes four points in the same plane.  It
     * checks to see if the line from pt0 to pt1 crosses the
     * line from pt2 to pt3. */

    for (i = 0 ; i < 3; i++) {
     vect1[i] = pt1[i] - pt0[i] ;
     vect2[i] = pt2[i] - pt0[i] ;
     vect3[i] = pt3[i] - pt0[i] ;
    }

    CrossProd (vect2, vect1, cross) ;
    t2 = (cross[0]*normal[0]+cross[1]*normal[1]+cross[2]*normal[2]) ;
    CrossProd (vect3, vect1, cross) ;
    t3 = (cross[0]*normal[0]+cross[1]*normal[1]+cross[2]*normal[2]) ;

    if (t2*t3 < 0.0) {
        CrossProd (vect2, vect3, cross) ;
        t = (cross[0]*normal[0]+cross[1]*normal[1]+cross[2]*normal[2]) ;

        result = (t * t2 > 0.0) ;

        if (result) {
            return(1) ;
        }
    }

    return(0) ;
}

/* -----------------------------------------------------------------------
**
**    IntersectFaceWithPlane - intersect a face with a plane
**
**      void IntersectFaceWithPlane(
**              int       sign[3],
**              double    face_coords[3][3],
**              double    face_norm,
**              double    face_d,
**              double    plane_norm[3],
**              double    plane_d,
**              double    intsct[2][3])
**
**        sign        - (in)  sign of the dot product of the vectors
**                            from the face's vertices to the plane and
**                            the normal to the plane (tells on which
**                            side of the plane the points lie).
**        face_coords - (in)  coordinates for the face
**        face_norm   - (in)  normal to the face
**        face_d      - (in)  constant to the face equation
**        plane_norm  - (in)  the normal to the plane
**        plane_d     - (in)  constant in the plane equation
**        intsct      - (out) intersection points
**
**      Description: Finds the points where the edges of a face
**          intersect a plane.
** -- */

void IntersectFaceWithPlane(int sign[3],double face_coords[3][3],
		            double face_norm[3],  double face_d,
			    double plane_norm[3], double plane_d,
			    double intsct[2][3])
{
    int cur = 0, dir, i ;
    double diff[3], det, d, norm[3] ;
    double E1, E2, E3, F1, F2, F3 ;
    double detx, dety, detz ;

    for (i=0 ; i<3 ; ++i) {
        if (sign[i] == 0) {
            if (sign[(i+1)%3] == 0) {
                intsct[0][0] = face_coords[i][0] ;
                intsct[0][1] = face_coords[i][1] ;
                intsct[0][2] = face_coords[i][2] ;
                intsct[1][0] = face_coords[(i+1)%3][0] ;
                intsct[1][1] = face_coords[(i+1)%3][1] ;
                intsct[1][2] = face_coords[(i+1)%3][2] ;
                break ;
            } else {
                intsct[cur][0] = face_coords[i][0] ;
                intsct[cur][1] = face_coords[i][1] ;
                intsct[cur][2] = face_coords[i][2] ;
                ++cur ;
            }
        }
        if (sign[i]*sign[(i+1)%3] < 0) {

            /* we have two nodes that straddle the front face plane.
             * We want to find the plane that passes through this
             * edge and is perpendicular to one of the coordinate
             * axis.  Once we have this we can solve the 3x3 system
             * made up of the plane equation for this plane and
             * the planes of the two faces to find the coordinates
             * of the intersection.
             *
             * As there may be more than one plane for us to choose
             * from, we use the one that gives the larges determinate
             * for the 3x3 system. */

            diff[0] = face_coords[(i+1)%3][0] - face_coords[i][0] ;
            diff[1] = face_coords[(i+1)%3][1] - face_coords[i][1] ;
            diff[2] = face_coords[(i+1)%3][2] - face_coords[i][2] ;

            E1 = face_norm[0] * plane_norm[1] ;
            E2 = face_norm[1] * plane_norm[2] ;
            E3 = face_norm[2] * plane_norm[0] ;

            F1 = plane_norm[1] * face_norm[2] ;
            F2 = plane_norm[2] * face_norm[0] ;
            F3 = plane_norm[0] * face_norm[1] ;

            detx = diff[1] * (F3-E1) + diff[2] * (E3-F2) ;
            dety = diff[2] * (F1-E2) + diff[0] * (E1-F3) ;
            detz = diff[0] * (F2-E3) + diff[1] * (E2-F1) ;

            if (fabs(detx) >= fabs(dety)) {
                dir = (fabs(detx) >= fabs(detz)) ? 0 : 2 ;
            } else {
                dir = (fabs(dety) >= fabs(detz)) ? 1 : 2 ;
            }

            d = 0.0 ;
            det = 0.0 ;
            switch (dir) {
                case 0:
                    det = detx ;
		    norm[0] = 0.0 ;
		    norm[1] = diff[2] ;
		    norm[2] = -diff[1] ;
                    d = diff[2]*face_coords[i][1] -
                        diff[1]*face_coords[i][2] ;
                    break ;
                case 1:
                    det = dety ;
		    norm[0] = -diff[2] ;
		    norm[1] = 0.0 ;
		    norm[2] = diff[0] ;
                    d = diff[0]*face_coords[i][2] -
                        diff[2]*face_coords[i][0] ;
                    break ;
                case 2:
                    det = detz ;
		    norm[0] = diff[1] ;
		    norm[1] = -diff[0] ;
		    norm[2] = 0.0 ;
                    d = diff[1]*face_coords[i][0] -
                        diff[0]*face_coords[i][1] ;
                    break ;
            }

            ThreePlaneIntersect(face_norm,face_d,plane_norm,plane_d,
                                norm,d,det,intsct[cur]) ;
            ++cur ;
        }
    }
}

/*
** -----------------------------------------------------------------------
**
**    ThreePlaneIntersect - intersect planes
**
**      double *ThreePlaneIntersect(
**              double norm0[3],
**              double    d0,
**              double norm1[3],
**              double    d1,
**              double norm2[3],
**              double    d2,
**              double    det,
**              double    intsct[3])
**
**        norm0  - (in)  first plane normal
**        d0     - (in)  first plane constant
**        norm1  - (in)  second plane normal
**        d1     - (in)  second plane constant
**        norm2  - (in)  third plane normal
**        d2     - (in)  third plane constant
**        det    - (in)  determinate to use in solving the system (see
**                       comments in IntersectFaceWithPlane).
**        intsct - (out) intersection points
**
**      Description: This method finds the pointe where three planes
**          intersect.
**
**      Return Value: intersection coordinate
**
**
** -- */

void ThreePlaneIntersect(double norm0[3],double d0,
                         double norm1[3],double d1,
                         double norm2[3],double d2,
                         double det,double intsct[3])
{
    double inv[3][3] ;

    inv[0][0] =  (norm1[1] * norm2[2] -
                  norm1[2] * norm2[1]) / det ;
    inv[0][1] = -(norm0[1] * norm2[2] -
                  norm0[2] * norm2[1]) / det ;
    inv[0][2] =  (norm0[1] * norm1[2] -
                  norm0[2] * norm1[1]) / det ;
    inv[1][0] = -(norm1[0] * norm2[2] -
                  norm1[2] * norm2[0]) / det ;
    inv[1][1] =  (norm0[0] * norm2[2] -
                  norm0[2] * norm2[0]) / det ;
    inv[1][2] = -(norm0[0] * norm1[2] -
                  norm0[2] * norm1[0]) / det ;
    inv[2][0] =  (norm1[0] * norm2[1] -
                  norm1[1] * norm2[0]) / det ;
    inv[2][1] = -(norm0[0] * norm2[1] -
                  norm0[1] * norm2[0]) / det ;
    inv[2][2] =  (norm0[0] * norm1[1] -
                  norm0[1] * norm1[0]) / det ;

    intsct[0] = (inv[0][0]*d0 + inv[0][1]*d1 + inv[0][2]*d2) ;
    intsct[1] = (inv[1][0]*d0 + inv[1][1]*d1 + inv[1][2]*d2) ;
    intsct[2] = (inv[2][0]*d0 + inv[2][1]*d1 + inv[2][2]*d2) ;
}


/* -------------------------------------------------------------------
**
**    LineFaceIntersect - intersect a line and a face
**
**      int LineFaceIntersect(
**              double pt0[3],
**              double pt1[3],
**              double fp0[3],
**              double fp1[3],
**              double fp2[3],
**              double *t,
**              double tol)
**
**        pt0  - (in)  first  line endpoint
**        pt1  - (in)  second line endpoint
**        fp0  - (in)  first  node of face to intersect
**        fp1  - (in)  second node of face to intersect
**        fp2  - (in)  third  node of face to intersect
**        t    - (out) normalized position (0-1) between line endpoints
**                     of the intersection point
**        tol  - (in)  tolerance (in parametric coordinates space) to
**                     use to check for intersection point inside the
**                     face
**
**      Description: Find the point of intersection (if any) between a
**          line and a face.
**
**      Return Value: true if they intersect
**
** -- */

int LineFaceIntersect(double pt0[3],
                      double pt1[3],
		      double fp0[3],
		      double fp1[3],
		      double fp2[3],
                      double *ti,
                      double tol)
{
    int    i ;
    double tmp0, tmp1, len, t ;
    double intsct[3], v0[3], v1[3] ;
    double r, s, u, vect[3] ;
    double fnorm[3], fd, farea ;
    double cross[3], prod ;
    double rf[3], sf[3], r_x_s[3], vec[3] ;

    /* Compute face normal, the constant for plane equation for face
     * to be intersected and the area of this face */

    TripleCrossProd (fp0, fp1, fp2, fnorm) ;
    len = sqrt (fnorm[0]*fnorm[0]+fnorm[1]*fnorm[1]+fnorm[2]*fnorm[2]) ;
    if (fabs(len) >= 1e-12) {
     fnorm[0] /= len ;
     fnorm[1] /= len ;
     fnorm[2] /= len ;
    }
    fd = fnorm[0]*fp0[0] + fnorm[1]*fp0[1] + fnorm[2]*fp0[2] ;
    for (i = 0; i < 3; i++) {
     rf[i] = fp1[i] - fp0[i] ;
     sf[i] = fp2[i] - fp0[i] ;
    }
    r_x_s[0] = rf[1] * sf[2] - sf[1] * rf[2] ;
    r_x_s[1] = rf[2] * sf[0] - sf[2] * rf[0] ;
    r_x_s[2] = rf[0] * sf[1] - sf[0] * rf[1] ;
    farea = sqrt( r_x_s[0] * r_x_s[0] +
 		  r_x_s[1] * r_x_s[1] +
		  r_x_s[2] * r_x_s[2] ) / 2.0 ;

    /* find the point of intersection between the line
     * and the plane of the face */

    tmp0 = (fnorm[0]*pt0[0] + fnorm[1]*pt0[1] + fnorm[2]*pt0[2]) ;
    tmp1 = (fnorm[0]*pt1[0] + fnorm[1]*pt1[1] + fnorm[2]*pt1[2]) ;
    vect[0] = pt1[0] - pt0[0] ;
    vect[1] = pt1[1] - pt0[1] ;
    vect[2] = pt1[2] - pt0[2] ;
    len = sqrt (vect[0]*vect[0]+vect[1]*vect[1]+vect[2]*vect[2]) ;
    if (fabs(tmp0-tmp1) <= 1e-12*len) return(0) ;
    t = (tmp0 - fd) / (tmp0 - tmp1) ;
    *ti = t ;

    intsct[0] = pt0[0] + t*(pt1[0]-pt0[0]) ;
    intsct[1] = pt0[1] + t*(pt1[1]-pt0[1]) ;
    intsct[2] = pt0[2] + t*(pt1[2]-pt0[2]) ;

    /* check to see if the intersection point is inside the
     * triangle by computing the area baricentric coordinates
     * and checking to see if they are all > 0 and < 1. */

    v0[0] = intsct[0] - fp0[0] ;
    v0[1] = intsct[1] - fp0[1] ;
    v0[2] = intsct[2] - fp0[2] ;
    v1[0] = fp1[0] - fp0[0] ;
    v1[1] = fp1[1] - fp0[1] ;
    v1[2] = fp1[2] - fp0[2] ;
    CrossProd (v0, v1, cross) ;
    prod = (cross[0]*fnorm[0] + cross[1]*fnorm[1] + cross[2]*fnorm[2]) ;
    r = 0.5 * prod / farea ;

    v1[0] = fp2[0] - fp0[0] ;
    v1[1] = fp2[1] - fp0[1] ;
    v1[2] = fp2[2] - fp0[2] ;
    CrossProd (v0, v1, cross) ;
    prod = (cross[0]*fnorm[0] + cross[1]*fnorm[1] + cross[2]*fnorm[2]) ;
    s = 0.5 * prod / farea ;
    u = 1.0 - r - s ;

    if ((r >= -tol) && (r <= 1.0+tol) &&
        (s >= -tol) && (s <= 1.0+tol) &&
        (u >= -tol) && (u <= 1.0+tol)) return(1) ;
    return(0) ;
}


/* --------------------------------------------------------------------
**
**    CheckCross - check for crossing lines
**
**      int CheckCross(
**              double i1[2],
**              double i2[2],
**              double j1[2],
**              double j2[2])
**
**        i1 - (in)  first coordinate
**        i2 - (in)  second coordinat
**        j1 - (in)  third coordinat
**        j2 - (in)  forth coordinat
**
**      Description: Checks to see if the line between the first two
**          points crosses the line between the second two points.
**
**      Return Value: true (1) if they cross
**
**
** -- */

int CheckCross(double i1[2],double i2[2], double j1[2],double j2[2])
{
    /* This is the assumed node configuration:

           \I1   /J2
            \   /
             \ /
              \
             / \
            /   \
           /J1   \I2
    */

    double a[2], b[2], c[2], d[2] ;
    double cross_ab, cross_cd ;

    /* Now compute the cross product of line I with J1 and line I
     * with J2.  If have the same sign the lines cannot cross */

    a[0] = i2[0]-i1[0] ;
    a[1] = i2[1]-i1[1] ;
    b[0] = j1[0]-i1[0] ;
    b[1] = j1[1]-i1[1] ;
    cross_ab = a[0]*b[1] - a[1]*b[0] ;
    c[0] = i2[0]-i1[0] ;
    c[1] = i2[1]-i1[1] ;
    d[0] = j2[0]-i1[0] ;
    d[1] = j2[1]-i1[1] ;
    cross_cd = c[0]*d[1] - c[1]*d[0] ;
    if ((cross_ab * cross_cd) >= 0)
        return (0) ;

    /* find the cross product of line J with I1 and line J with I2.
     * If they have the same sign, the lines cannot cross */

    a[0] = j2[0]-j1[0] ;
    a[1] = j2[1]-j1[1] ;
    b[0] = i1[0]-j1[0] ;
    b[1] = i1[1]-j1[1] ;
    cross_ab = a[0]*b[1] - a[1]*b[0] ;
    c[0] = j2[0]-j1[0] ;
    c[1] = j2[1]-j1[1] ;
    d[0] = i2[0]-j1[0] ;
    d[1] = i2[1]-j1[1] ;
    cross_cd = c[0]*d[1] - c[1]*d[0] ;
    if ((cross_ab * cross_cd) >= 0)
        return (0) ;

    return (0) ;
}

/*
** ----------------------------------------------------------------------
**
** CrossProd - Given two vectors, compute cross product.
*/

double *CrossProd(double a[3], double b[3], double n[3])
{
 n[0] = (a[1]*b[2]) - (a[2]*b[1]) ;
 n[1] = (a[2]*b[0]) - (a[0]*b[2]) ;
 n[2] = (a[0]*b[1]) - (a[1]*b[0]) ;
 return n;
}

/*
** ----------------------------------------------------------------------
**
** TripleCrossProd - Given three nodes, compute cross product.
*/

double *TripleCrossProd (double a[3], double b[3], double c[3], double n[3])
{
 n[0] = (b[1]-a[1])*(c[2]-a[2]) - (b[2]-a[2])*(c[1]-a[1]);
 n[1] = (b[2]-a[2])*(c[0]-a[0]) - (b[0]-a[0])*(c[2]-a[2]);
 n[2] = (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
 return n;
}


/* -------------------------------------------------------------------
** Msh3DDetNode  - this routine determines the node that should be
**                 taken in case of two nodes choosen has the same
**                 coordinates.
*/

static int Msh3DDetNode(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int                next_indx )
{
    int             above ;
    Msh3DAdjIniFace cur ;

    /* look if the face center is above of planes defined by all
       adjacent faces of node_indx. If it's, then node_indx is
       the node that should be choosen. */

    above = 1 ;
    for( cur = node_list[node_indx].ifaces ; cur ; cur = cur->next )
    {
     if( !PtAboveHalfPlane( cur->verts[0], cur->r_x_s, face->center ) ) {
      above = 0 ;
      break ;
     }
    }
    if( above ) return node_indx ;

    /* look if the face center is above of planes defined by all
       adjacent faces of next_indx. If it's, then next_indx is
       the node that should be choosen. */

    above = 1 ;
    for( cur = node_list[next_indx].ifaces ; cur ; cur = cur->next )
    {
     if( !PtAboveHalfPlane( cur->verts[0], cur->r_x_s, face->center ) ) {
      above = 0 ;
      break ;
     }
    }
    if( above ) return next_indx ;

    /* if we get here, both nodes can not be choosen */

    return -1 ;
}

/* -------------------------------------------------------------------
** Msh3DAddFaces - this routine updates the face stack to include/
**                 delete the necessary boundary faces to include
**                 the new element.  It also updates the active
**                 flags in the node list.
*/

static void Msh3DAddFaces(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int               *num_elems )
{
    int            v1i, v2i, v1, v2, i, j ;
    int            found, made ;
    int            in_pt_tree[4] ;
    Msh3DAdjFace   cur, *temp, save ;
    Msh3DBdryFace  face_to_delete, new, current, fac ;
    PointCoord     pt ;
    RangeCoord     min, max ;

    /* start with each of the base nodes.  See if they are adjacent
       to a face that includes the cap node along with the previous
       vertex on the base.  If so, this face is already part of the
       boundary, and it should be deleted. */

#if DSP_MESH
    if ( FDEBUG)
      GdbSetCurVertexOn (node_indx);
#endif

#if defined(PT_TREE) || defined(PT_DEBU)
    /* check if nodes are already in the PT tree before adding faces */
    for (i = 0; i < 3; i++) {
     if ( !node_list[face->verts[i]].faces )
      in_pt_tree[i] = 0 ; /* not in PT tree before adding faces */
     else
      in_pt_tree[i] = 1 ; /* in PT tree before adding faces */
    }
    if ( !node_list[node_indx].faces )
     in_pt_tree[3] = 0 ; /* not in PT tree before adding faces */
    else
     in_pt_tree[3] = 1 ; /* in PT tree before adding faces */
#endif

    /* update the current front, adding or deleting faces */
    for ( v1i=1,v2i=0 ; v2i<3 ; v1i=((v1i+1)%3), v2i++ ) {
        v1 = face->verts[v1i] ;
        v2 = face->verts[v2i] ;
        for ( cur=node_list[v1].faces ; cur ; cur=cur->next ) {
            for ( i=found=0 ; (i<3) && !found ; i++ ) {
                if (( cur->face->verts[i] == v1 ) &&
                    ( cur->face->verts[(i+1)%3] == v2 ) &&
                    ( cur->face->verts[(i+2)%3] == node_indx ))
                    found = 1 ;
            }
            if ( found ) break ;
        }

       /* if we have found a face, first we update all the
           adjacent face lists so they no longer point to it.
           Then we delete the face from the face list */

        if ( found ) {
            /* update adjacent lists */
            face_to_delete = cur->face ;
            for ( temp = &(node_list[v1].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }
            for ( temp = &(node_list[v2].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }
            for ( temp = &(node_list[node_indx].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }

#if DSP_MESH
            if ( FDEBUG && GdbWait) {
               GdbClear();
               GdbSetTopOff(GDB_NEWFACE);
               GdbSetDelFaceOn (cur->face);
               GdbDrawBdry ();
            }
#endif

	    /* save old deleted faces */
#if OLD_TEST
            fac = Msh3DTestPush() ;
	    fac->verts[0] = cur->face->verts[0] ;
	    fac->verts[1] = cur->face->verts[1] ;
	    fac->verts[2] = cur->face->verts[2] ;
#endif

	    /* remove this face from range tree */
#if defined(AD_TREE) || defined(AD_DEBU)
	    min[0] = cur->face->min[0] ;
	    min[1] = cur->face->min[1] ;
	    min[2] = cur->face->min[2] ;
	    max[0] = cur->face->max[0] ;
	    max[1] = cur->face->max[1] ;
	    max[2] = cur->face->max[2] ;
	    RemoveFromRangeTree( min, max, cur->face->adtreeindx ) ;
	    adtreehash[cur->face->adtreeindx] = 0 ;
#endif

	    /* add the new element in the face data structure. If
	     * one element was already created (rgn0), then this
	     * is the second one being created (rgn1). */
#if OLD_ELEM
	    cur->face->rgn[0] = -1 ;
	    cur->face->rgn[1] = -1 ;
#else
	    if (cur->face->rgn[0] == -1) cur->face->rgn[0] = (*num_elems)-1 ;
	    else                         cur->face->rgn[1] = (*num_elems)-1 ;
#endif

	    /* delete this face from the front */
            Msh3DBdryDelete( cur->face ) ;
        }

        /* if we did not find the face, we must create it and update
           all the adjacent lists */

        else {

            /* add the new face to the front */
            new = Msh3DPushBdryFace( node_list, v2, v1, node_indx, face->use, 1 ) ;

            /* add the new element in the face data structure. If
	     * one element was already created (rgn[0]), then this
	     * is the second one (rgn[1]). */
#if OLD_ELEM
	    new->rgn[0] = -1 ;
	    new->rgn[1] = -1 ;
#else
	    if (new->rgn[0] == -1) new->rgn[0] = (*num_elems)-1 ;
	    else                   new->rgn[1] = (*num_elems)-1 ;
#endif

	    /* insert this face into the range tree */
#if defined(AD_TREE) || defined(AD_DEBU)
	    min[0] = new->min[0] ;
	    min[1] = new->min[1] ;
	    min[2] = new->min[2] ;
	    max[0] = new->max[0] ;
	    max[1] = new->max[1] ;
	    max[2] = new->max[2] ;
	    new->adtreeindx = adtreeindx ;
	    AddToRangeTree (min, max, adtreeindx) ;
	    if (adtreeindx >= adtreesize) {
             adtreesize += MSH3D_ADTREE_QUANTUM ;
	     adtreehash = (Msh3DBdryFace *) Msh3DRealloc (adtreehash,
			           adtreesize*sizeof(Msh3DBdryFace)) ;
            }
	    adtreehash[adtreeindx] = new ;
	    ++adtreeindx ;
#endif

	    /* check for old deleted faces */
#if OLD_TEST
          made = 0 ;
	  fac = test_cursor ;
	  while( fac ) {
           if (( fac->verts[0] == v2 ) &&
               ( fac->verts[1] == v1 ) &&
               ( fac->verts[2] == node_indx ))
            made = 1 ;
           if ( made ) break ;
	   fac = fac->next ;
          }
          fac = Msh3DTestPush() ;
	  fac->verts[0] = v2 ;
	  fac->verts[1] = v1 ;
	  fac->verts[2] = node_indx ;
#endif

#if DSP_MESH
            if ( FDEBUG && GdbWait ) {
               GdbClear();
               GdbSetTopOff(GDB_DELFACE);
               GdbSetNewFaceOn (new);
               GdbDrawBdry ();
            }
#endif

	    /* update adjacency */
            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new ;
            cur->next = node_list[v1].faces ;
            node_list[v1].faces = cur ;

            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new ;
            cur->next = node_list[v2].faces ;
            node_list[v2].faces = cur ;

            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new ;
            cur->next = node_list[node_indx].faces ;
            node_list[node_indx].faces = cur ;
        }
    }

    /* update all the adjacent face lists so they no longer point
       to the base face */

    for ( i=0 ; i<3 ; i++ ) {
        for ( temp = &(node_list[face->verts[i]].faces) ; *temp ;
              temp = &((*temp)->next) ) {
            if ( (*temp)->face == face ) {
                save = *temp ;
                *temp = (*temp)->next ;
                Msh3DAdjFree( save ) ;
                break ;
            }
        }
    }

    /* update the active flags.  Any node on the new element that
       no longer adjacent to at least on face on the boundary
       becomes inactive. Also, add or remove the nodes from the
       point tree if necessary: a) if the node before updating the
       front was in the PT tree (had adjacent faces) and now
       it is not in the tree anymore (doesn't have adjacent faces
       anymore) then this node should be removed from tree; b) if
       the node was not before updating the front was not in
       the PT tree (didn't have adjacent faces) and now it is
       in the tree (now has adjacent faces), then this node should
       be included to the tree */

#if defined(PT_TREE) || defined(PT_DEBU)
    for ( i=0 ; i<3 ; i++ ) {
        if ( !node_list[face->verts[i]].faces ) {
            node_list[face->verts[i]].active_flag = 0 ;
	    if (in_pt_tree[i]) {
             pt[0] = node_list[face->verts[i]].coord[0] ;
             pt[1] = node_list[face->verts[i]].coord[1] ;
             pt[2] = node_list[face->verts[i]].coord[2] ;
	     RemoveFromPointTree( pt, face->verts[i] ) ;
	    }
        }
    }
    if ( !node_list[node_indx].faces ) {
            node_list[node_indx].active_flag = 0 ;
	    if (in_pt_tree[3]) {
             pt[0] = node_list[node_indx].coord[0] ;
             pt[1] = node_list[node_indx].coord[1] ;
             pt[2] = node_list[node_indx].coord[2] ;
	     RemoveFromPointTree( pt, node_indx ) ;
            }
    }
    else {
            node_list[node_indx].active_flag = 1 ;
	    if (!in_pt_tree[3]) {
             pt[0] = node_list[node_indx].coord[0] ;
             pt[1] = node_list[node_indx].coord[1] ;
             pt[2] = node_list[node_indx].coord[2] ;
  	     AddToPointTree( pt, node_indx ) ;
            }
    }
#else
    for ( i=0 ; i<3 ; i++ ) {
        if ( !node_list[face->verts[i]].faces )
            node_list[face->verts[i]].active_flag = 0 ;
    }
    if ( !node_list[node_indx].faces )
            node_list[node_indx].active_flag = 0 ;
    else
            node_list[node_indx].active_flag = 1 ;
#endif

//added by markos
#if defined(AD_TREE) || defined(AD_DEBU)
	    min[0] = face->min[0] ;
	    min[1] = face->min[1] ;
	    min[2] = face->min[2] ;
	    max[0] = face->max[0] ;
	    max[1] = face->max[1] ;
	    max[2] = face->max[2] ;
	    RemoveFromRangeTree( min, max, face->adtreeindx ) ;
	    adtreehash[face->adtreeindx] = 0 ;
#endif
//added by markos

    /* give up the memory associated with this face */

#if OLD_TEST
    fac = Msh3DTestPush() ;
    fac->verts[0] = face->verts[0] ;
    fac->verts[1] = face->verts[1] ;
    fac->verts[2] = face->verts[2] ;
#endif

    /* add the new element in the face data structure. If
     * one element was already created (rgn0), then this
     * is the second one being created (rgn1). */

#if OLD_ELEM
    face->rgn[0] = -1 ;
    face->rgn[1] = -1 ;
#else
    if (face->rgn[0] == -1) face->rgn[0] = (*num_elems)-1 ;
    else                    face->rgn[1] = (*num_elems)-1 ;
#endif

    /* delete the face from the front */

    Msh3DBdryFree( face ) ;
}

/* -------------------------------------------------------------------
** Msh3DAddPolyFaces - this routine updates the boundary and polyhedron
**                     stacks to include/delete the necessary boundary
**                     and polyhedron faces to include the new element.
**                     It also updates the active flags in the node list.
*/

static void Msh3DAddPolyFaces(
    Msh3DPolyFace      poly,
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx,
    int               *num_elems,
    int                pjmesh_use_box_test )
{
    int            v1i, v2i, v1, v2, i, j ;
    int            found, made ;
    Msh3DAdjFace   cur, *temp, save ;
    Msh3DBdryFace  face_to_delete, new_bdry, current, fac ;
    Msh3DPolyFace  new_poly ;

    /* start with each of the base nodes.  See if they are adjacent
       to a face that includes the cap node along with the previous
       vertex on the base.  If so, this face is already part of the
       boundary, and it should be deleted. */

#if DSP_MESH
    if ( FDEBUG)
      GdbSetCurVertexOn (node_indx);
#endif

    for ( v1i=1,v2i=0 ; v2i<3 ; v1i=((v1i+1)%3), v2i++ ) {
        v1 = face->verts[v1i] ;
        v2 = face->verts[v2i] ;
        for ( cur=node_list[v1].faces ; cur ; cur=cur->next ) {
            for ( i=found=0 ; (i<3) && !found ; i++ ) {
                if (( cur->face->verts[i] == v1 ) &&
                    ( cur->face->verts[(i+1)%3] == v2 ) &&
                    ( cur->face->verts[(i+2)%3] == node_indx ))
                    found = 1 ;
            }
            if ( found ) break ;
        }

       /* if we have found a face, first we update all the
           adjacent face lists so they no longer point to it.
           Then we delete the face from the face list */

        if ( found ) {
            face_to_delete = cur->face ;
            for ( temp = &(node_list[v1].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }
            for ( temp = &(node_list[v2].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }
            for ( temp = &(node_list[node_indx].faces) ; *temp ;
                  temp = &((*temp)->next) ) {
                if ( (*temp)->face == face_to_delete ) {
                    save = *temp ;
                    *temp = (*temp)->next ;
                    Msh3DAdjFree( save ) ;
                    break ;
                }
            }

#if DSP_MESH
            if ( FDEBUG && GdbWait) {
               GdbClear();
               GdbSetTopOff(GDB_NEWFACE);
               GdbSetDelFaceOn (cur->face);
               GdbDrawBdry ();
            }
#endif

	    /* store information for repeated faces */
#if OLD_TEST
            fac = Msh3DTestPush() ;
            fac->verts[0] = face->verts[0] ;
            fac->verts[1] = face->verts[1] ;
            fac->verts[2] = face->verts[2] ;
#endif

            /* add the new element in the face data structure. If
             * one element was already created (rgn[0]), then this
             * is the second one (rgn[1]). */
#if OLD_ELEM
            cur->face->rgn[0] = -1 ;
            cur->face->rgn[1] = -1 ;
#else
            if (cur->face->rgn[0] == -1) cur->face->rgn[0] = (*num_elems)-1 ;
            else                         cur->face->rgn[1] = (*num_elems)-1 ;
#endif

            /* delete the face from the polyhedron */
            Msh3DPolyDelete( cur->face ) ;

	    /* delete this face from the front */
            Msh3DBdryDelete( cur->face ) ;
        }

        /* if we did not find the face, we must create it and update
           all the adjacent lists */

        else {

          new_bdry = Msh3DPushBdryFace(node_list,v2,v1,node_indx,face->use,1) ;
          new_poly = Msh3DPushPolyFace(new_bdry) ;

#if OLD_ELEM
	  new_bdry->rgn[0] = -1 ;
	  new_bdry->rgn[1] = -1 ;
#else
	  if (new_bdry->rgn[0] == -1) new_bdry->rgn[0] = (*num_elems)-1 ;
	  else                        new_bdry->rgn[1] = (*num_elems)-1 ;
#endif

#if OLD_TEST
          made = 0 ;
	  fac = test_cursor ;
	  while( fac ) {
           if (( fac->verts[0] == v2 ) &&
               ( fac->verts[1] == v1 ) &&
               ( fac->verts[2] == node_indx ))
            made = 1 ;
           if ( made ) break ;
	   fac = fac->next ;
          }
          fac = Msh3DTestPush() ;
          fac->verts[0] = v2 ;
          fac->verts[1] = v1 ;
          fac->verts[2] = node_indx ;
#endif

#if DSP_MESH
            if ( FDEBUG && GdbWait ) {
               GdbClear();
               GdbSetTopOff(GDB_DELFACE);
               GdbSetNewFaceOn (new_bdry);
               GdbDrawBdry ();
            }
#endif

            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new_bdry ;
            cur->next = node_list[v1].faces ;
            node_list[v1].faces = cur ;

            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new_bdry ;
            cur->next = node_list[v2].faces ;
            node_list[v2].faces = cur ;

            cur = Msh3DAdjFaceAlloc() ;
            cur->face = new_bdry ;
            cur->next = node_list[node_indx].faces ;
            node_list[node_indx].faces = cur ;
        }
    }

    /* update all the adjacent face lists so they no longer point
       to the base face */

    //added by markos
    if ((!pjmesh_use_box_test) || (!PMsh3DIsInvertedFace(face)))
    //end added by markos
    {
		for ( i=0 ; i<3 ; i++ ) {
			for ( temp = &(node_list[face->verts[i]].faces) ; *temp ;
				  temp = &((*temp)->next) ) {
				if ( (*temp)->face == face ) {
					save = *temp ;
					*temp = (*temp)->next ;
					Msh3DAdjFree( save ) ;
					break ;
				}
			}
		}
    }

    /* update the active flags.  Any node on the new element that
       no longer adjacent to at least on face on the boundary
       becomes inactive */

    for ( i=0 ; i<3 ; i++ ) {
        if ( !node_list[face->verts[i]].faces )
            node_list[face->verts[i]].active_flag = 0 ;
    }
    if ( !node_list[node_indx].faces )
            node_list[node_indx].active_flag = 0 ;
    else
            node_list[node_indx].active_flag = 1 ;

    /* give up the memory associated with this face */

#if OLD_TEST
    fac = Msh3DTestPush() ;
    fac->verts[0] = face->verts[0] ;
    fac->verts[1] = face->verts[1] ;
    fac->verts[2] = face->verts[2] ;
#endif

    /* add the new element in the face data structure. If
     * one element was already created (rgn[0]), then this
     * is the second one (rgn[1]). */
#if OLD_ELEM
     face->rgn[0] = -1 ;
     face->rgn[1] = -1 ;
#else
     if (face->rgn[0] == -1) face->rgn[0] = (*num_elems)-1 ;
     else                    face->rgn[1] = (*num_elems)-1 ;
#endif

     /* delete this face from the polyhedron */
     Msh3DPolyDelete( face ) ;

     /* delete this face from the front */
	//commented by markos
     //Msh3DBdryDelete( face ) ;
     //end commented by markos
/* PJMesh - delete face from front only if it is not inverted */
	if ((pjmesh_use_box_test) && (PMsh3DIsInvertedFace(face)))
	{
		PMsh3DInvertFace(face);
	}
	else
	{
		Msh3DBdryDelete(face);
	}
/* end PJMesh - delete face from front only if it is not inverted */
}

/* -------------------------------------------------------------------
** Msh3DHeap - these routines manage a priorty queue using a heap
**             data structure.
*/

typedef struct _Msh3DHeapEntry {
    double   value ;
    int      indx ;
    }
    Msh3DHeapEntry, *Msh3DHeap ;

static Msh3DHeap the_heap = 0 ;
static int       alloced_size ;
static int       heap_last ;

static void Msh3DHeapInit( int size )
{
    /* make sure we have enough room for the heap */

    if ( !the_heap ) {
        the_heap = (Msh3DHeap)Msh3DMalloc( size * sizeof(Msh3DHeapEntry) ) ;
        alloced_size = size ;
    }
    else if ( alloced_size < size ) {
        Msh3DFree( the_heap ) ;
        the_heap = (Msh3DHeap)Msh3DMalloc( size * sizeof(Msh3DHeapEntry) ) ;
        alloced_size = size ;
    }
    heap_last = -1 ;
}

static void Msh3DHeapDelete(void)
{
    Msh3DFree( the_heap ) ;
    the_heap = 0 ;
}

static void Msh3DHeapInsert( double value, int indx )
{
    int  cur, parent ;

    /* put the new values at the end of the heap */

    heap_last++ ;
    the_heap[heap_last].value = value ;
    the_heap[heap_last].indx = indx ;

    /* perform a shift up operation to restore the heap property */

    cur = heap_last ;
    while ( cur != 0 ) {
        parent = cur >> 1 ;    /* fast integer division by 2 */
        if ( the_heap[parent].value <= the_heap[cur].value ) break ;
        value = the_heap[parent].value ;  /* else swap */
        indx = the_heap[parent].indx ;
        the_heap[parent].value = the_heap[cur].value ;
        the_heap[parent].indx = the_heap[cur].indx ;
        the_heap[cur].value = value ;
        the_heap[cur].indx = indx ;
        cur = parent ;
    }
}

static int Msh3DHeapExtract( double *value )
{
    int    rtn_indx, cur, child, indx ;
    double tmp_value ;

    if ( heap_last < 0 ) return( -1 ) ;

    /* extract the element at the top of the heap */

    rtn_indx = the_heap[0].indx ;
    *value = the_heap[0].value ;

    /* replace this element with the one at the bottom of the heap */

    the_heap[0].indx = the_heap[heap_last].indx ;
    the_heap[0].value = the_heap[heap_last].value ;
    heap_last-- ;

    /* shift down to restore the heap order */

    cur = 0 ;
    child = 2 * cur ;
    while ( child <= heap_last ) {
        if ( child+1 <= heap_last ) {
            if ( the_heap[child+1].value < the_heap[child].value )
               child = child + 1 ;
        }
        if ( the_heap[cur].value <= the_heap[child].value ) break ;
        tmp_value = the_heap[child].value ;  /* else swap */
        indx = the_heap[child].indx ;
        the_heap[child].value = the_heap[cur].value ;
        the_heap[child].indx = the_heap[cur].indx ;
        the_heap[cur].value = tmp_value ;
        the_heap[cur].indx = indx ;
        cur = child ;
        child = 2 * cur ;
    }

    return( rtn_indx ) ;
}

static double Msh3DHeapSolidAngle(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                v0,
    int		       v1,
    int                v2,
    int                indx )
{
    int                i, n_vert, type ;
    double             *verts[3], *p1, *p2, *cen ;
    double             area = 0.0, mod, dot ;
    double             r[3], s[3], p[3], r_x_s[3], a[3], r1[3], b[3] ;
    double             edge, height, dist, cand_vec[3], den, num ;

    /* get the index vertex */

    cen = node_list[indx].coord ;

    /* put the face vertices in a polygon structure */

    n_vert = 3 ;
    verts[0] = node_list[v0].coord ;
    verts[1] = node_list[v1].coord ;
    verts[2] = node_list[v2].coord ;

    /* find the normal of the polygon */

    r_x_s[0] = r_x_s[1] = r_x_s[2] = 0.0 ;
    r[0] = verts[1][0]-verts[0][0] ;
    r[1] = verts[1][1]-verts[0][1] ;
    r[2] = verts[1][2]-verts[0][2] ;
    for ( i = 2; i < n_vert ; i++ )
    {
     s[0] = verts[i][0]-verts[0][0] ;
     s[1] = verts[i][1]-verts[0][1] ;
     s[2] = verts[i][2]-verts[0][2] ;
     p[0] = r[1] * s[2] - s[1] * r[2] ;
     p[1] = r[2] * s[0] - s[2] * r[0] ;
     p[2] = r[0] * s[1] - s[0] * r[1] ;
     r_x_s[0] += p[0] ;
     r_x_s[1] += p[1] ;
     r_x_s[2] += p[2] ;
     r[0] = s[0] ;
     r[1] = s[1] ;
     r[2] = s[2] ;
    }
    mod = sqrt( r_x_s[0]*r_x_s[0] + r_x_s[1]*r_x_s[1] + r_x_s[2]*r_x_s[2] ) ;
    r_x_s[0] /= mod ;
    r_x_s[1] /= mod ;
    r_x_s[2] /= mod ;

    /* verify if the index vertex is too near from the polygon plane */

    edge = sqrt( (4*face->area) / sqrt(3.0) );
    height = (edge * sqrt(13.0)) / 4.0;
    cand_vec[0] = cen[0] - face->center[0] ;
    cand_vec[1] = cen[1] - face->center[1] ;
    cand_vec[2] = cen[2] - face->center[2] ;
    num = cand_vec[0] * face->r_x_s[0] +
          cand_vec[1] * face->r_x_s[1] +
          cand_vec[2] * face->r_x_s[2] ;
    den = face->r_x_s[0] * face->r_x_s[0] +
          face->r_x_s[1] * face->r_x_s[1] +
          face->r_x_s[2] * face->r_x_s[2] ;
    dist = fabs(num) / sqrt(den) ;

//fprintf (stderr,"markos debug, dist = %f, height = %f, height/10 = %f\n", dist, height, (height/(10.0*1.0))) ;

    if ( dist <= (height/(10.0*1.0)) )
      return 0.0 ;

    /* find the angle solid that the index vertex makes with the polygon */

    p2 = verts[n_vert-1] ;  /* last  vertex */
    p1 = verts[0] ;         /* first vertex */
    a[0] = p2[0]-p1[0], a[1] = p2[1]-p1[1], a[2] = p2[2]-p1[2] ;

    for ( i = 0; i < n_vert; i++ )
    {
     r1[0] = p1[0]-cen[0], r1[1] = p1[1]-cen[1], r1[2] = p1[2]-cen[2] ;
     p2 = verts[(i+1)%n_vert] ;
     b[0] = p2[0] - p1[0], b[1] = p2[1] - p1[1], b[2] = p2[2] - p1[2] ;

     area += Msh3DHeapSolidEdge( a, b, r1, r_x_s ) ;

     a[0] = -1.0*b[0] ; a[1] = -1.0*b[1] ; a[2] = -1.0*b[2] ;
     p1 = p2 ;
    }

    area -= PI*(n_vert-2) ;
    dot = ( r_x_s[0] * r1[0] + r_x_s[1] * r1[1] + r_x_s[2] * r1[2] ) ;
    return (dot>0.0) ? (-area) : (area) ;
}

static double Msh3DHeapSolidAngleTria(
    int                v0,
    int		       v1,
    int                v2,
    int                indx )
{
    double              *c0, *c1, *c2, *cen ;
    double              area = 0.0, mod, dplane, dot ;
    double              r[3], s[3], r_x_s[3], a[3], r1[3], b[3] ;

    /* get the index vertex */

    cen = node_list[indx].coord ;

    /* get the triangle vertices */

    c0  = node_list[v0].coord ;
    c1  = node_list[v1].coord ;
    c2  = node_list[v2].coord ;

    /* find the normal of the triangle */

    r[0] = c1[0]-c0[0] ; r[1] = c1[1]-c0[1] ; r[2] = c1[2]-c0[2] ;
    s[0] = c2[0]-c1[0] ; s[1] = c2[1]-c1[1] ; s[2] = c2[2]-c1[2] ;
    r_x_s[0] = r[1] * s[2] - s[1] * r[2] ;
    r_x_s[1] = r[2] * s[0] - s[2] * r[0] ;
    r_x_s[2] = r[0] * s[1] - s[0] * r[1] ;
    mod = sqrt( r_x_s[0]*r_x_s[0] + r_x_s[1]*r_x_s[1] + r_x_s[2]*r_x_s[2] ) ;
    r_x_s[0] /= mod ;
    r_x_s[1] /= mod ;
    r_x_s[2] /= mod ;
    dplane = -((r_x_s[0]*c0[0])+(r_x_s[1]*c0[1])+(r_x_s[2]*c0[2])) ;

    /* find the angle solid that the index vertex makes with the triangle */

    a[0] = c2[0]-c0[0] ; a[1] = c2[1]-c0[1] ; a[2] = c2[2]-c0[2] ;
    r1[0] = cen[0]-c0[0] ; r1[1] = cen[1]-c0[1] ; r1[2] = cen[2]-c0[2] ;
    b[0] = c1[0]-c0[0] ; b[1] = c1[1]-c0[1] ; b[2] = c1[2]-c0[2] ;
    area += Msh3DHeapSolidEdge( a, b, r1, r_x_s ) ;

    a[0] = -1.0*b[0] ; a[1] = -1.0*b[1] ; a[2] = -1.0*b[2] ;
    r1[0] = cen[0]-c1[0] ; r1[1] = cen[1]-c1[1] ; r1[2] = cen[2]-c1[2] ;
    b[0] = c2[0]-c1[0] ; b[1] = c2[1]-c1[1] ; b[2] = c2[2]-c1[2] ;
    area += Msh3DHeapSolidEdge( a, b, r1, r_x_s ) ;

    a[0] = -1.0*b[0] ; a[1] = -1.0*b[1] ; a[2] = -1.0*b[2] ;
    r1[0] = cen[0]-c2[0] ; r1[1] = cen[1]-c2[1] ; r1[2] = cen[2]-c2[2] ;
    b[0] = c0[0]-c2[0] ; b[1] = c0[1]-c2[1] ; b[2] = c0[2]-c2[2] ;
    area += Msh3DHeapSolidEdge( a, b, r1, r_x_s ) ;

    area -= PI ;
    dot = ( r_x_s[0] * r1[0] + r_x_s[1] * r1[1] + r_x_s[2] * r1[2] ) ;
    return (dot>0.0) ? (-area) : (area) ;
}

static double Msh3DHeapSolidEdge(
    double              a[3],
    double              b[3],
    double              r1[3],
    double              r_x_s[3] )
{
    double              n1[3], n2[3], l1, l2, s, ang, ba[3] ;

    n1[0] = a[1] * r1[2] - r1[1] * a[2] ;
    n1[1] = a[2] * r1[0] - r1[2] * a[0] ;
    n1[2] = a[0] * r1[1] - r1[0] * a[1] ;
    n2[0] = r1[1] * b[2] - b[1] * r1[2] ;
    n2[1] = r1[2] * b[0] - b[2] * r1[0] ;
    n2[2] = r1[0] * b[1] - b[0] * r1[1] ;
    l1 = sqrt( n1[0] * n1[0] + n1[1] * n1[1] + n1[2] * n1[2] ) ;
    l2 = sqrt( n2[0] * n2[0] + n2[1] * n2[1] + n2[2] * n2[2] ) ;
    s = (( n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2] ) / ( l1 * l2 )) ;
    ang = acos(MAX(-1.0,MIN(1.0,s))) ;
    ba[0] = b[1] * a[2] - a[1] * b[2] ;
    ba[1] = b[2] * a[0] - a[2] * b[0] ;
    ba[2] = b[0] * a[1] - a[0] * b[1] ;
    s = ( ba[0] * r_x_s[0] + ba[1] * r_x_s[1] + ba[2] * r_x_s[2] ) ;
    return (s>0.0) ? (PI-ang) : (PI+ang) ;
}

/* -------------------------------------------------------------------
** Msh3DDecomPoly - these routines implement element extraction procedure.
*/

static int Msh3DDecomPoly (
  int                *nnodes,
  int                *nelems,
  int                **elems,
  int                gdb)
{
  int           i = 0;
  int           v, v1, v2, v3, v4 ;
  int           ni, extr_fail ;
  Msh3DBdryFace face, f1, f2, ftmp ;
  int           aux ;

  if (!bdry_stack) return 1 ;

  /* initiate graphic package use to debug and display
     the initial surface discretization ans internal points if
     flag is actived */

#if DSP_MESH
  if ( FDEBUG && gdb ) {
    GdbIni ("Mesh3d Generator - Element Extraction Technique");
    GdbDrawBdry();
  }
  fprintf (stderr,"Element Extraction\n");
#endif

  /* generate the mesh by element extraction procedure */

  while (bdry_stack && i < 1000) {
    i++;
    while ( (v = ExtractVert ((*nnodes),node_list,&v1,&v2,&v3)) != -1 ) {
      face = Msh3DCrt1FDelV (node_list,v,v1,v2,v3) ;
      if (face==NULL) {
        return 0;
      }
      ftmp = (Msh3DBdryFace)malloc(sizeof(Msh3DBdryFaceRec));
      ftmp->verts[0] = face->verts[0];
      ftmp->verts[1] = face->verts[2];
      ftmp->verts[2] = face->verts[1];
      Msh3DAddElem( nelems, elems, ftmp, v ) ;
      free(ftmp);
      if (face==bdry_stack) {
       Msh3DBdryDelete ( face );
       break;
      }
    }
    if ( bdry_stack == NULL )
     extr_fail = 0 ;
    else
     extr_fail = 1 ;

    if (extr_fail)
    {
     if (!ExtractEdge (node_list,&f1,&f2,&v1,&v2) ) {
      extr_fail = 1 ;
     }
     else
     {
      extr_fail = 0 ;
      face = Msh3DCrt2FDelE (node_list,f1,f2,v1,v2) ;
      ftmp = (Msh3DBdryFace)malloc(sizeof(Msh3DBdryFaceRec));
      ftmp->verts[0] = face->verts[0];
      ftmp->verts[1] = face->verts[2];
      ftmp->verts[2] = face->verts[1];
      Msh3DAddElem( nelems, elems, ftmp, v2 ) ;
      free(ftmp);
     }
    }

    if (extr_fail)
    {
     if (((*nnodes)+1) >= node_alloc) {
      node_alloc += node_quantum ;
      node_list = (Msh3DBdryNodeList)Msh3DRealloc( node_list,
		    node_alloc * sizeof(Msh3DBdryNodeRec) ) ;
     }
     if (!ExtractFace (node_list,&f1,&v1,nnodes) ) {
      return 0 ;
     }
     else
     {
      face = Msh3DCrt3FDelF (node_list,f1,v1,&ni) ;
      ftmp = (Msh3DBdryFace)malloc(sizeof(Msh3DBdryFaceRec));
      ftmp->verts[0] = face->verts[0];
      ftmp->verts[1] = face->verts[2];
      ftmp->verts[2] = face->verts[1];
      Msh3DAddElem( nelems, elems, ftmp, ni ) ;
      free(ftmp);
     }
    }
  }

#if DSP_MESH
  if ( FDEBUG) {
    GdbClear ();
    GdbSetWait(0);
    GdbDrawBdry ();
  }
#endif
  return 1;
}

static Msh3DBdryFace Msh3DCrt1FDelV( Msh3DBdryNodeList node_list,
		                     int v, int v1, int v2, int v3 )
{
  int            vcur[3], i, j;
  Msh3DAdjFace   adj, *temp, save ;
  Msh3DBdryFace  face_to_delete, new ;


  /* delete all faces adjacent to v and update all the adjacent face
  ** lists so they no longer point to them.
  */

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
     GdbSetTopOff (GDB_EDGE);
     GdbSetCurVertexOn (v);
  }
#endif

  vcur[0] = v1; vcur[1] = v2; vcur[2] = v3;
  for ( i=0, adj=node_list[v].faces; adj ; i++, adj=adj->next ) {

    if (i>3) {
      /* fprintf (stderr,"error on removal vertex operator\n"); */
      return NULL;
    }
    face_to_delete = adj->face ;
    for ( j=0; j<3; j++ ) {
      for ( temp = &(node_list[vcur[j]].faces) ; *temp ;
            temp = &((*temp)->next) ) {

        if ( (*temp)->face == face_to_delete ) {
           save = *temp ;
           *temp = (*temp)->next ;
           Msh3DAdjFree( save ) ;
           break ;
        }
      }
    }

#if DSP_MESH
    if ( FDEBUG && GdbWait ) {
      GdbSetDelFaceOn (face_to_delete);
    }
#endif

    Msh3DBdryDelete( face_to_delete ) ;
  }

  /* return the stack if it's the last tetrahedral */

  if ( bdry_stack->next == NULL )
     return bdry_stack;

  /* we must create it and update all the adjacent lists */

  new = Msh3DPushBdryFace( node_list, v1, v2, v3, 0, 1 ) ;

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
     GdbSetNewFaceOn (new);
  }
#endif

  /* update all the adjacent lists */

  for ( j=0; j<3; j++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new ;
    adj->next = node_list[vcur[j]].faces ;
    node_list[vcur[j]].faces = adj ;
  }

  /* update the active flags. */

  node_list[v].active_flag = 0 ;

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
     GdbClear ();
     GdbDrawBdry ();
  }
#endif

  return new;
}

static Msh3DBdryFace Msh3DCrt2FDelE(
  Msh3DBdryNodeList  node_list,
  Msh3DBdryFace      f1,
  Msh3DBdryFace      f2,
  int                v1,
  int                v2 )
{
  int            vcur[3], i;
  int            v3, v4;
  Msh3DAdjFace   adj, *temp, save ;
  Msh3DBdryFace  new1, new2 ;


  for ( i=0 ; i<3; i++ )
    if ( f1->verts[i] == v1 )
      v3 = f1->verts[(i+2)%3];

  for ( i=0 ; i<3; i++ )
    if ( f2->verts[i] == v1 )
      v4 = f2->verts[(i+1)%3];

  /* we must create 2 faces and update all the adjacent lists */

  new1 = Msh3DPushBdryFace( node_list, v1, v4, v3, 0, 1 ) ;
  new2 = Msh3DPushBdryFace( node_list, v2, v3, v4, 0, 1 ) ;

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
     GdbSetTopOff (GDB_VERTEX);
     GdbSetCurEdgeOn (v1,v2);
     GdbSetNewFaceOn (new1);
     GdbSetNewFaceOn (new2);
     GdbSetDelFaceOn (f1);
     GdbSetDelFaceOn (f2);
  }
#endif

  /* delete all faces adjacent to v and update all the adjacent face
  ** lists so they no longer point to them.
  */

  vcur[0] = v1; vcur[1] = v2; vcur[2] = v3;
  for ( i=0; i<3; i++ ) {
    for ( temp = &(node_list[vcur[i]].faces) ; *temp ;
          temp = &((*temp)->next) ) {

      if ( (*temp)->face == f1 ) {
         save = *temp ;
         *temp = (*temp)->next ;
         Msh3DAdjFree( save ) ;
         break ;
      }
    }
  }
  Msh3DBdryDelete( f1 ) ;

  vcur[0] = v1; vcur[1] = v4; vcur[2] = v2;
  for ( i=0; i<3; i++ ) {
    for ( temp = &(node_list[vcur[i]].faces) ; *temp ;
          temp = &((*temp)->next) ) {

      if ( (*temp)->face == f2 ) {
         save = *temp ;
         *temp = (*temp)->next ;
         Msh3DAdjFree( save ) ;
         break ;
      }
    }
  }
  Msh3DBdryDelete( f2 ) ;

  /* update all the adjacent lists */

  vcur[0] = v1; vcur[1] = v4; vcur[2] = v3;
  for ( i=0; i<3; i++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new1 ;
    adj->next = node_list[vcur[i]].faces ;
    node_list[vcur[i]].faces = adj ;
  }

  /* update all the adjacent lists */

  vcur[0] = v2; vcur[1] = v3; vcur[2] = v4;
  for ( i=0; i<3; i++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new2 ;
    adj->next = node_list[vcur[i]].faces ;
    node_list[vcur[i]].faces = adj ;
  }

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
     GdbClear ();
     GdbDrawBdry ();
  }
#endif

  return new1;
}

static Msh3DBdryFace Msh3DCrt3FDelF(
  Msh3DBdryNodeList  node_list,
  Msh3DBdryFace      f1,
  int                node_indx,
  int                *ni_indx )
{
  int            vcur[3], i;
  int            v1, v2, v3;
  Msh3DAdjFace   adj, *temp, save ;
  Msh3DBdryFace  new1, new2, new3 ;

  /* we must create 3 faces and update all the adjacent lists */

  v1 = f1->verts[0] ;
  v2 = f1->verts[1] ;
  v3 = f1->verts[2] ;

  new1 = Msh3DPushBdryFace( node_list, v1, v2, node_indx, 0, 1 ) ;
  new2 = Msh3DPushBdryFace( node_list, v2, v3, node_indx, 0, 1 ) ;
  new3 = Msh3DPushBdryFace( node_list, v3, v1, node_indx, 0, 1 ) ;

#if DSP_MESH
  if ( FDEBUG ) {
     GdbSetTopOff (GDB_EDGE);
     GdbSetCurVertexOn (node_indx);
     GdbSetDelFaceOn (f1);
     GdbSetNewFaceOn (new1);
     GdbSetNewFaceOn (new2);
     GdbSetNewFaceOn (new3);
  }
#endif

  /* delete all faces adjacent to v and update all the adjacent face
  ** lists so they no longer point to them.
  */

  vcur[0] = v1; vcur[1] = v2; vcur[2] = v3;
  for ( i=0; i<3; i++ ) {
    for ( temp = &(node_list[vcur[i]].faces) ; *temp ;
          temp = &((*temp)->next) ) {

      if ( (*temp)->face == f1 ) {
         save = *temp ;
         *temp = (*temp)->next ;
         Msh3DAdjFree( save ) ;
         break ;
      }
    }
  }
  Msh3DBdryDelete( f1 ) ;

  /* update all the adjacent lists */

  vcur[0] = v1; vcur[1] = v2; vcur[2] = node_indx;
  for ( i=0; i<3; i++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new1 ;
    adj->next = node_list[vcur[i]].faces ;
    node_list[vcur[i]].faces = adj ;
  }

  /* update all the adjacent lists */

  vcur[0] = v2; vcur[1] = v3; vcur[2] = node_indx;
  for ( i=0; i<3; i++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new2 ;
    adj->next = node_list[vcur[i]].faces ;
    node_list[vcur[i]].faces = adj ;
  }

  /* update all the adjacent lists */

  vcur[0] = v3; vcur[1] = v1; vcur[2] = node_indx;
  for ( i=0; i<3; i++ ) {
    adj = Msh3DAdjFaceAlloc() ;
    adj->face = new3 ;
    adj->next = node_list[vcur[i]].faces ;
    node_list[vcur[i]].faces = adj ;
  }

#if DSP_MESH
  if ( FDEBUG ) {
     GdbClear ();
     GdbDrawBdry ();
  }
#endif

  *ni_indx = v3;
  return new1;
}

/* -------------------------------------------------------------------
** Msh3DCorrePoly - these routines implement correct polyhedron procedure.
*/

static int Msh3DCorrePoly (
  int                *nnodes,
  int                *nelems,
  int                **elems,
  int                gdb,
  int                pok,
  int                shp,
  int                dsp,
  int                pjmesh_use_box_test)
{
  Msh3DPolyFace poly, poly_face, cur, inter ;
  Msh3DBdryFace face, visi, bdry_face ;
  Msh3DBdryNode node ;
  Msh3DAdjFace  *temp, save, adj_face ;
  double        cand_vec[3], dot, len, size = 1.0e+7 ;
  double        metric, next_metric ;
  int           v0, v1, v2 ;
  int           i, j, k ;
  int           x, y, z ;
  int           num_org_nodes ;
  int           num_org_faces ;
  double        original_nodes[1000][3] ;
  int           original_faces[1000][3] ;
  int           first, vert, found, check, valid, stop ;
  int           corre, diare, alloc, trials, face_pop = 0 ;
  int           node_indx, next_indx, node_poly, star_poly ;
  int           nface_node = 0 ;
  int           nface_star = 0 ;
  int           nface_push = 0 ;
  int           nface_face = 0 ;
  int           *face_node ;
  int           *face_star ;
  int           *face_push ;
  Msh3DBdryFace *face_face ;

  if (!bdry_stack) return 1 ;

  /* initiate graphic package use to debug and display
     the initial surface discretization for polyhedron */

#if DSP_MESH
  if ( FDEBUG && gdb ) {
    GdbIni ("Mesh3d Generator - Star-Shaping Technique");
    GdbDrawBdry();
    gdb = 0 ;
  }
#endif

#if DSP_PRNT
  fprintf (stderr,"Correct Polyhedron\n");
#endif

  /* allocate face vectors */

  face_node = (int *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(int) ) ;
  face_star = (int *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(int) ) ;
  face_push = (int *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(int) ) ;
  face_face = (Msh3DBdryFace *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(Msh3DBdryFace) ) ;

  /* generate the mesh by correct polyhedron procedure */

  //added by markos
  face = bdry_stack ;
  //end added by markos

  //commented and added by markos
  //while (bdry_stack) {
  while (face) {
  //end commented and added by markos

#if DSP_TIME
    if (!shp)
     init_time = clock( );
#endif

    /* find the polyhedron for bdry_stack face. If it is the back-tracking
     * phase, use FindPoly, which treats pathological cases of nodes and
     * edges shared. If it is the improvement phase, use Reconstruct Poly
     * which do the standard construction, not treating these cases. */

    //commented by markos
    //face = bdry_stack ;
    //end commented by markos

    //added by markos
	if (pjmesh_use_box_test)
	{
		//while ((face) && (face->use != 2))
		while ((face) && (face->use == -1))
		{
			face = face->next;
		}

		if (!face)
		{
			break;
		}
	}
	//end added by markos

	//debug markos
	/*if ((face->verts[0] == 320) && (face->verts[1] == 492) && (face->verts[2] == 627) ||
		(face->verts[0] == 110) && (face->verts[1] == 577) && (face->verts[2] == 581) ||
		(face->verts[0] == 420) && (face->verts[1] == 429) && (face->verts[2] == 581) ||
		//(face->verts[0] == 582) && (face->verts[1] == 446) && (face->verts[2] == 456) ||
		//(face->verts[0] == 606) && (face->verts[1] == 637) && (face->verts[2] == 654) ||
		false
		)
	{
		fprintf(stdout, "found face face %d, %d, %d\n", face->verts[0], face->verts[1], face->verts[2]);
	}
	else
	{
		face = face->next;

		continue;
	}*/
	//endebug

	//debug markos
	//if ((face->verts[0] == 582) && (face->verts[1] == 446) && (face->verts[2] == 456))
	//{
	//	fprintf(stdout, "found face face %d, %d, %d\n", face->verts[0], face->verts[1], face->verts[2]);
	//}
	//endebug markos

	//debug markos
	//PMsh3DPrintBdryList("\n\n\nwill now work to back track face (before Msh3DRecoPoly)\n", face, 1, 0, 1);
	//endebug markos

	//PMsh3DPrintBdryList("\nbefore Msh3DFindPoly\n", face, 0, 1, 1);

    if (!shp) {
#if OLD_FIND
     if ( jmesh_version == SMALL_VERSION ) /* Use improved find for SMALL */
      Msh3DFindPoly (face,face_face,face_node,&nface_face,&nface_node,shp, pjmesh_use_box_test) ;
     else                                  /* Use standard find for ADVAN */
      Msh3DRecoPoly (face,face_face,face_node,&nface_face,&nface_node,shp, pjmesh_use_box_test) ;
#else
     Msh3DFindPoly (face,face_face,face_node,&nface_face,&nface_node,shp, pjmesh_use_box_test) ;
#endif
    }
    else {
     Msh3DRecoPoly (face,face_face,face_node,&nface_face,&nface_node,shp, pjmesh_use_box_test) ;
    }
    valid = 0 ;
    corre = 0 ;
    diare = 0 ;
    node_poly = -1 ;

    //PMsh3DPrintBdryList("\nafter Msh3DFindPoly\n", face, 0, 1, 1);

    //debug markos
    //PMsh3DPrintBdryList("\n\n\nwill now work to back track face (after Msh3DRecoPoly)\n", face, 1, 1);
    //endebug markos

    /* use the face on the top of the polyhedron stack. If the stack is
       empty then we go to another polyhedron */

#if DSP_MESH
  if ( FDEBUG && gdb ) {
    GdbIni ("Mesh3d Generator - Star-Shaping Technique");
    GdbDrawBdry();
    gdb = 0;
  }
  else if (dsp) {
    GdbFit ( ) ;
    GdbClear();
    GdbDrawBdry();
  }
#endif

#if DSP_PRNT
  fprintf (stderr,"Correct Polyhedron\n");
#endif

	/*//markos debug
	printf("CorrePoly with face %d %d %d\n", face->verts[0], face->verts[1], face->verts[2]);
	//vendebug*/

    while (poly = Msh3DPolyPop() ) {

/*//markos debug
if ((poly->face->verts[0] == 615) && (poly->face->verts[1] == 692) && (poly->face->verts[2] == 686))
{
	printf("poly->face %d %d %d\n", poly->face->verts[0], poly->face->verts[1], poly->face->verts[2]);
}
//markos endebug*/

#if DSP_MESH
  if ( FDEBUG && GdbWait && dsp ) {
    GdbReset();
    GdbSetCurFaceOn(poly->face);
    GdbClear();
  }
#endif

     /* here we have two options for the correct polyhedron procedure:
        the first  one is to use star-shaped polyhedron procedure  and
        the second one is to use boundary contraction   procedure. The
        first one is currently being used because gives better results,
        unless there are crack faces in the polyhedron and then boundary
        contraction procedure should be used because the star-shaped
        polyhedron procedure will be expensive to be used. */

     star_poly = Msh3DStarPoly (nnodes, *elems, nelems, node_poly) ;

     if( star_poly ) {

      /* examine all active nodes. Rank all nodes as to the goodness
         of the tetrahedral they will form with with the current face.
         Here a consideration should be done: in fact, the right thing
         to do is only consider the node included in the polyhedron and
         that is visible from all faces; another approach is consider
         all nodes as possible candidates, that should be used when
         the first approach fails after some polyhedron corrections. */

    if ( node_poly == -1 ) {

     /* start with the node with the best ranking.  Check to make
        sure it will form a valid element (i.e., does not intersect
        the current boundary).  If the element is invalid go on
        to the next.  If the element is valid, then look at the next
        candidate node.  If the two nodes have the same coordinates
        then use topology to determine the one we want. */

     trials = 0 ;
     valid  = 0 ;
     alloc  = 0 ;
     visi   = 0 ;
     while (!valid) {

     /* Try insertion of a node by polyhedron criteria */

     node_indx = -1 ;

     /* Try insertion of new node by a polyhedron procedure. If
        it's not possible, correct the polyhedron and try again */

     if (node_indx == -1) {

      /* no more nodes avaiable in the heap. Try insertion of new ones */

      next_indx =  1 ;
      alloc     =  1 ;

      /* try insertion of new node */

      for (k=0; k<=POL_NODES; k++)
      {
       /* node inserted */

       check = Msh3DInsertNewNodesPol (poly->face, nnodes, k,
                 &alloc, &face_pop, dsp) ;

       /* check validity for node inserted */

       if (check)
        check = Msh3DCheckVisib( (*nnodes)-1, &visi ) ;
       if (check)
        check = Msh3DCheckPoly( node_list, (*nnodes)-1, -1, 1, *elems,
                                nelems, &visi, dsp ) ;
       if (check)
        check = Msh3DCheckPoint( (*nnodes)-1, pok ) ;

       /* it's not necessary try anymore if node is valid */

       if (check) break ;
      }

      /* set validity for node inserted */

      valid = check ;
      if (valid) {
       node_indx = (*nnodes) - 1 ;
#if DIA_POLY
       if (!shp) {
        fprintf (stderr, "Worked with NewNodesPol:\n") ;
        fprintf (stderr, "Number of iteractions = %d\n", k) ;
       }
#endif
      }
      else {
       (*nnodes)-- ;
       nintnode-- ;
      }
      if (next_indx == -1) break ;
     }

     /* here no node was found to make the element (no node_indx was
        found). Try insertion of new node by a adjacent procedure. If
        it's not possible, correct the polyhedron and try again */

     if (node_indx == -1) {

      /* no more nodes avaiable in the heap. Try insertion of new ones */

      next_indx =  -1 ;
      alloc     =   1 ;

      /* try insertion of new node */

      for (k=1; k<=POL_NODES; k++)
      {
       for (cur = poly_stack; cur != poly_stack->next; cur = cur->next)
       {
        /* get the face */

        face = cur->face ;

        /* node inserted */

        check = Msh3DInsertNewNodesAdj (face, nnodes, k,
                  &alloc, &face_pop, dsp) ;

        /* check validity for node inserted */

        if (check)
         check = Msh3DCheckVisib( (*nnodes)-1, &visi ) ;
        if (check)
         check = Msh3DCheckPoly( node_list, (*nnodes)-1, -1, 1, *elems,
                                 nelems, &visi, dsp ) ;
        if (check)
         check = Msh3DCheckPoint( (*nnodes)-1, pok ) ;

        /* it's not necessary try anymore if node is valid */

        if (check) break ;
       }
       if (check) break ;
      }

      /* set validity for node inserted */

      valid = check ;
      if (valid) {
       node_indx = (*nnodes) - 1 ;
#if DIA_POLY
       if (!shp) {
        fprintf (stderr, "Worked with NewNodesAdj:\n") ;
        fprintf (stderr, "Number of iteractions = %d\n", k) ;
       }
#endif
      }
      else {
       (*nnodes)-- ;
       nintnode-- ;
      }
      if (next_indx == -1) break ;
     }

     /* here no node was found to make the element (no node_indx was
        found). Try insertion of new node by a optimal procedure,
	using only the boundary faces. This is important because if
	we have more than one boundary face, we are dangerously close
	to the boundary and something has to be done. */

#if 0
     if (node_indx == -1) {

      /* no more nodes avaiable in the heap. Try insertion of new ones */

      next_indx =  1 ;
      alloc     =  1 ;

      /* try insertion of new node */

      for (k=1; k<=POL_NODES; k++)
      {
       /* node inserted */

       check = Msh3DInsertNewNodesBdr (poly->face, nnodes, k,
                 &alloc, &face_pop, dsp) ;

       /* check validity for node inserted */

       if (check)
        check = Msh3DCheckVisib( (*nnodes)-1, &visi ) ;
       if (check)
        check = Msh3DCheckPoly( node_list, (*nnodes)-1, -1, 1, *elems,
                                nelems, &visi, dsp ) ;
       if (check)
        check = Msh3DCheckPoint( (*nnodes)-1, pok ) ;

       /* it's not necessary try anymore if node is valid */

       if (check) break ;
      }

      /* set validity for node inserted */

      valid = check ;
      if (valid) {
       node_indx = (*nnodes) - 1 ;
#if DIA_POLY
       if (!shp) {
        fprintf (stderr, "Worked with NewNodesBdr:\n") ;
        fprintf (stderr, "Number of iteractions = %d\n", k) ;
       }
#endif
      }
      else {
       (*nnodes)-- ;
       nintnode-- ;
      }
      if (next_indx == -1) break ;
     }
#endif

    }

   }
   else
   {
    /* get the node inserted for the polyhedron. No check to make
       sure it will form a valid element (i.e., does not intersect
       the current boundary) is necessary. The element is valid
       because the test for this node was already done before when
       this node was created. */

       node_indx = node_poly ;
       if ((node_indx == poly->face->verts[0]) ||
           (node_indx == poly->face->verts[1]) ||
           (node_indx == poly->face->verts[2]))
       {
        while (cur = Msh3DPolyPop() ) {
         bdry_face = cur->face ;
         Msh3DPolyDelete( bdry_face ) ;
         face = Msh3DBdryPopCorre( bdry_face ) ;
         Msh3DBdryPush( face ) ;
        }
        poly_stack = poly_tail = NULL ;

        break ;
       }
   }

    /* if no node was found even with the insertion of new nodes by
       procedures described above and the number of allowed corrections
       was not reached, correct the polyhedron to try again, else return
       to boundary contraction with the code error. Else update nface_node
       that will be used if another polyhedron exists */

    if  ( node_indx == -1 ) {

     /* Correct the polyhedron */

     //debug markos
     //PMsh3DPrintBdryList("\n\n\nwill now work to back track face (before Msh3DCrtnFDelF)\n", face, 1, 0, 1);
     //endebug markos

     //PMsh3DPrintBdryList("\nbefore Msh3DCrtnFDelF\n", visi, 0, 1, 1);

#if VER_BOTH
     Msh3DCrtnFDelF (visi, *elems, nelems, corre, shp, pjmesh_use_box_test) ;
     corre++ ;
     diare++ ;
     if (!shp) {
      stop = MSH3D_POLY_FACE_CORRE ;
      if (corre > stop) {
#if VER_PRNT
       fprintf( stderr, "	Returning because back-tracking failed!!\n") ;
       fprintf( stderr, "	Deleted   elements: %8d ele.\n", diare);
#endif
       Msh3DFree( face_node ) ;
       Msh3DFree( face_face ) ;
       Msh3DFree( face_star ) ;
       Msh3DFree( face_push ) ;
       return 0 ;
      }
      if ( (corre == 300) || (corre == 600) ) {
#if DSP_TIME
       tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
       fprintf( stderr,"\t\t Working on back-tracking:\n") ;
       fprintf( stderr,"\t\t Modified  elements: %8d ele.\n", corre) ;
       fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
       fprintf( stderr,"\n") ;
#endif
      }
     }
#else
     Msh3DCrtnFDelF (visi, *elems, nelems, corre, shp, pjmesh_use_box_test) ;
     corre++ ;
#endif

	 //PMsh3DPrintBdryList("\nafter Msh3DCrtnFDelF\n", visi, 0, 1, 1);

	 //debug markos
	 //PMsh3DPrintBdryList("\n\n\nwill now work to back track face (after Msh3DCrtnFDelF)\n", face, 1, 0, 1);
	 //endebug markos

     /* If we are in the phase to improve the quality and not to
        generate a mesh and we've tried more than Msh3D_DELETE_ELEM
        times, then this element can not be improved and we should
        return, else continue trying to generate a mesh */

     if( shp )
     {
      if( num_elem_created >= Msh3D_DELETE_ELEM )
      {
       Msh3DFree( face_node ) ;
       Msh3DFree( face_face ) ;
       Msh3DFree( face_star ) ;
       Msh3DFree( face_push ) ;
       for( i = 0; i < num_elem_deleted; i++ )
       {
        for( j = 0; j < 4; j++ )
        {
         //added by markos
       	 int freedAll = 1;
       	 //end added by markos

         (*elems)[adj_elem_dele[i].elem*4+j] = adj_elem_dele[i].verts[j] ;
         for ( temp = &(node_list[adj_elem_dele[i].verts[j]].faces) ; *temp ;
             ) {
          save = *temp ;
          //commented by markos
          //*temp = (*temp)->next ;
          //end commented by markos

          //added by markos
          if ((pjmesh_use_box_test) && (PMsh3DIsInvertedFace(save->face)))
          {
          	freedAll = 0;
          	temp = &((*temp)->next);
          }
          else
          //end added by markos
          {
          	//added by markos
          	*temp = (*temp)->next ;
          	//end added by markos
          	Msh3DAdjFree( save ) ;
          }

         }
         //added by markos
         if ((!pjmesh_use_box_test) || (freedAll))
         //end added by markos
         {
         	node_list[adj_elem_dele[i].verts[j]].faces = 0 ;
			if ( !node_list[adj_elem_dele[i].verts[j]].faces )
			  node_list[adj_elem_dele[i].verts[j]].active_flag = 1 ;
         }
        }
       }
       //PMsh3DPrintBdryList("\nbefore while\n", NULL, 0, 1, 1);
       while (cur = Msh3DPolyPop() ) {
        bdry_face = cur->face ;
        Msh3DPolyDelete( bdry_face ) ;
        //commented by markos
        //Msh3DBdryDelete( bdry_face ) ;
        //end commented by markos
        //added by markos
        if ((!pjmesh_use_box_test) || (!PMsh3DIsInvertedFace(bdry_face)))
        {
			Msh3DBdryDelete( bdry_face ) ;
        }
        //debug
        else
        {
        	PMsh3DInvertFace(bdry_face);
        }
        //endebug
        //end added by markos
       }
       //PMsh3DPrintBdryList("\nafter while\n", NULL, 0, 1, 1);
       if( adj_elem_dele != 0 ) Msh3DFree( adj_elem_dele ) ;
       adj_elem_dele = 0 ;
       num_elem_deleted = 0 ;
       num_elem_created = 0 ;

/* PJMesh - deinvert faces of the remaining front */
		if (pjmesh_use_box_test)
		{
			PMsh3DInvertFaces();
		}
/* end PJMesh - deinvert faces of the remaining front */

//fprintf (stderr,"markos debug, improvment back tracking failed, tried to create too many elements\n") ;

       return 1 ;
      }
     }

     /* if we get here, we should continue with the new polyhedron */

     continue ;
   }
   else {
    node_poly  = node_indx ;
    corre      =  0 ;
    nface_node =  0 ;
   }

    /* add the new element to the element list, update the boundary and
       the polyhedron list, and update the active node list. */

       //PMsh3DPrintBdryList("\nbefore Msh3DAddElem\n", poly->face, 0, 1, 1);

       //debug markos
       //PMsh3DPrintBdryList("\n\n\nwill now work to advance face (advancing Msh3DCrtnFDelF)\n", poly->face, 1, 1, 1);
       //endebug markos

   Msh3DAddElem( nelems, elems, poly->face, node_indx ) ;
   Msh3DAddPolyFaces( poly, poly->face, node_list, node_indx, nelems, pjmesh_use_box_test ) ;

		//PMsh3DPrintBdryList("\nafter Msh3DAddElem\n", poly->face, 0, 1, 1);
  }
  else {

      /* find the faces of the boundary stack that are in the current
         polyhderon and the faces that arent't there. */

      Msh3DBdryReset() ;
      while ( bdry_face = Msh3DBdryNext() ) {
       found = 0 ;
       Msh3DPolyReset () ;
       while (poly_face = Msh3DPolyNext ()) {
        face = poly_face->face ;
        if (bdry_face == face) {
         found = 1 ;
         break ;
        }
       }
       if (!found) {
        face_star[nface_star*3+0] = bdry_face->verts[0] ;
        face_star[nface_star*3+1] = bdry_face->verts[1] ;
        face_star[nface_star*3+2] = bdry_face->verts[2] ;
        nface_star++ ;
       }
       else {
        face_push[nface_push*3+0] = bdry_face->verts[0] ;
        face_push[nface_push*3+1] = bdry_face->verts[1] ;
        face_push[nface_push*3+2] = bdry_face->verts[2] ;
        nface_push++ ;
       }
      }

      /* release current poly_stack */

      Msh3DPolyReset() ;
      while ( poly_face = Msh3DPolyNext() ) {
       bdry_face = poly_face->face ;
       Msh3DPolyDelete( bdry_face ) ;
      }
      poly_stack = poly_tail = NULL ;

      /* release current bdry_stack */

      while ( bdry_face = Msh3DBdryPop() ) {
       for ( j = 0; j < 3; j++ ) {
        vert = bdry_face->verts[j] ;
        for ( temp = &(node_list[vert].faces) ; *temp ;
              temp = &((*temp)->next) ) {
         if ( (*temp)->face == bdry_face ) {
          save = *temp ;
          *temp = (*temp)->next ;
          Msh3DAdjFree( save ) ;
          break ;
         }
        }
        if ( !node_list[vert].faces )
         node_list[vert].active_flag = 0 ;
       }
       Msh3DBdryFree( bdry_face ) ;
      }
      bdry_stack = bdry_tail = NULL ;

      /* release list of faces already made */

      Msh3DTestFreeAll() ;

      /* push all the boundary face descriptions onto a stack  */

      push_version = SMALL_VERSION ;

      for ( i=0 ; i<nface_push ; i++ )
          face = Msh3DPushBdryFace( node_list, face_push[i*3+0],
                 face_push[i*3+1], face_push[i*3+2], 0, 1 ) ;

      push_version = ADVAN_VERSION ;

      /* build the list of adjacent faces for all the boundary nodes */

      Msh3DBdryReset() ;
      while ( face = Msh3DBdryNext() ) {
        for ( j=0 ; j<3 ; j++ ) {
            node = &node_list[face->verts[j]] ;
            adj_face = Msh3DAdjFaceAlloc() ;
            adj_face->next = node->faces ;
            adj_face->face = face ;
            node->faces = adj_face ;
            node_list[face->verts[j]].active_flag = 1 ;
        }
      }

      /* find the tolerance based in the smallest edge of the model */

      Msh3DBdryReset() ;
      while ( face = Msh3DBdryNext() ) {
        for ( j=0 ; j<3 ; j++ ) {
         cand_vec[0] = node_list[face->verts[j]].coord[0] -
                       node_list[face->verts[(j+1)%3]].coord[0] ;
         cand_vec[1] = node_list[face->verts[j]].coord[1] -
                       node_list[face->verts[(j+1)%3]].coord[1] ;
         cand_vec[2] = node_list[face->verts[j]].coord[2] -
                       node_list[face->verts[(j+1)%3]].coord[2] ;
         len = sqrt ( cand_vec[0] * cand_vec[0] +
                      cand_vec[1] * cand_vec[1] +
                      cand_vec[2] * cand_vec[2] ) ;
         if( len < size ) size = len ;
        }
      }
#if OLD_TOLE
      tol_inters = TOL_INTERS * size ;
#else
      tol_inters = 1.0e-08 * size ;
#endif

      /* use the face on the top of the stack.  If the stack is
         empty then we are done */

      while ( face = Msh3DBdryPop() ) {

#if DSP_MESH
       if ( FDEBUG && GdbWait ) {
	if (dsp) {
         GdbReset();
         GdbSetCurFaceOn(face);
         GdbClear();
        }
       }
#endif

       /* examine all active nodes. Rank all nodes as to the goodness
          of the tetrahedral they will form with the current face */

       Msh3DHeapInit( (*nnodes) ) ;
       for ( i=0 ; i<(*nnodes) ; i++ )
       {
        if ( node_list[i].active_flag )
        {
         double d, dist, dist_vec[3], dnew[3], new[3] ;

         /* find the vector from the center to the new point
            and make sure that this dotted with the normal is
            positive */

         cand_vec[0] = node_list[i].coord[0] - face->center[0] ;
         cand_vec[1] = node_list[i].coord[1] - face->center[1] ;
         cand_vec[2] = node_list[i].coord[2] - face->center[2] ;

         dot = cand_vec[0]*face->r_x_s[0] +
               cand_vec[1]*face->r_x_s[1] +
               cand_vec[2]*face->r_x_s[2] ;

         if ( dot <= 0.0 ) continue ;

         /* verify if node is out of sphere centered in optimal node
            for the face and ratio equal of largest (smallest)
            distance from this node to any vertex of the face.
            This should be doen only the fase of ideal elements. */

         if (face->use == 0)
         {
          Msh3DFindNode (face, 1, new) ;
          dist = 1000000 ;
          for ( j=0; j<3; j++ ) {
           dnew[0] = new[0] - node_list[face->verts[j]].coord[0] ;
           dnew[1] = new[1] - node_list[face->verts[j]].coord[1] ;
           dnew[2] = new[2] - node_list[face->verts[j]].coord[2] ;
           d = sqrt ( dnew[0] * dnew[0] +
                      dnew[1] * dnew[1] +
                      dnew[2] * dnew[2] ) ;
           if ( d < dist ) dist = d ;
          }
          dist_vec[0] = node_list[i].coord[0] - new[0] ;
          dist_vec[1] = node_list[i].coord[1] - new[1] ;
          dist_vec[2] = node_list[i].coord[2] - new[2] ;
          d = sqrt ( dist_vec[0] * dist_vec[0] +
                     dist_vec[1] * dist_vec[1] +
                     dist_vec[2] * dist_vec[2] ) ;
          if ( d >= dist ) continue ;
         }

         /* the metric we are currently using is the square of
            the distance from the center of the face to the
            candidate node */

         if (face->use == 0)
         {
          metric = Msh3DHeapSolidAngle( face, node_list,
            face->verts[0], face->verts[1], face->verts[2], i ) ;
          if( metric != 0.0 )    /* multiply by -1 because of heap */
           Msh3DHeapInsert( (-1.0*metric), i ) ;
         }
         else
         {
          metric = 0.0 ;
          for ( j=0 ; j<3 ; j++ )
            metric += cand_vec[j] * cand_vec[j] ;

          metric = metric/dot ;
          Msh3DHeapInsert( metric, i ) ;
         }
        }
       }

       /* start with the node with the best ranking.  Check to make
          sure it will form a valid element (i.e., does not intersect
          the current boundary).  If the element is invalid go on
          to the next.  If the element is valid, then look at the next
          candidate node.  If the two nodes have the same coordinates
          then use topology to determine the one we want. */

        DebugFail = 0 ;
        trials = 0 ;
        valid = 0 ;
        alloc = 1 ;
        while ( !valid ) {

            /* extract a node from the heap based in its metric */

            node_indx = Msh3DHeapExtract( &metric ) ;
            if ((node_indx == face->verts[0]) ||
                (node_indx == face->verts[1]) ||
                (node_indx == face->verts[2])) continue ;

            /* here a node was found to make the element (node_indx was
               extracted from the heap). Check its validity */

            if ( node_indx != -1 ) {

             /* node avaiable in the heap */

             check = 1 ;

             /* check validity for node choosen */

             if (check)
              check = Msh3DCheckVolum( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckShape( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckValid( face, node_list, node_indx, -1,
                        -1, 0, *elems, nelems ) ;

             /* set validity for node choosen */

             valid = check ;
             if ( valid ) {
                 next_indx = Msh3DHeapExtract( &next_metric ) ;
                 if ( next_indx == -1 ) break ;
                 if (( next_metric == metric ) &&
                     ( node_list[node_indx].coord[0] ==
                       node_list[next_indx].coord[0] ) &&
                     ( node_list[node_indx].coord[1] ==
                       node_list[next_indx].coord[1] ) &&
                     ( node_list[node_indx].coord[2] ==
                       node_list[next_indx].coord[2] )) {
                    /* fprintf( stderr, "Determine The node we want \n" ) ; */
                    node_indx = Msh3DDetNode( face, node_list,
                                              node_indx, next_indx ) ;
                 }
             }
             else {
                trials += 1 ;
             }
            }

            /* here no node was found to make the element (no node_indx was
               found). Try insertion of new node by an baricentric procedure
               that represents the optimal node for this face */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = 1 ;
             alloc     = 1 ;

             /* try insertion of optimal node if it's in the phase of
                ideal elements */

             if (face->use == 0)
             {
               /* try insertion of new node */

               for (k=1; k<=1; k++)
               {
                /* node inserted */

                check = Msh3DInsertNewNodesBar (face, nnodes, k,
                          &alloc, &face_pop) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckOptim(face, (*nnodes)-1, num_org_nodes,
                           num_org_faces, original_nodes, original_faces) ;
                if (check)
                 check = Msh3DCheckValid( face, node_list, (*nnodes)-1,
                           -1, -1, 0, *elems, nelems ) ;

                 /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = (*nnodes) - 1 ;
               }
               else {
                (*nnodes)-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }

            /* here no node was found to make the element (no node_indx was
               found). Try insertion of new node by an adjacent procedure if
               this face is the first face that went one time to the end of
               the stack (bdry_try face represented by use equal to 1) */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = -1 ;
             alloc     =  1 ;

             /* try insertion of optimal node if it's in the phase of
                insert elements */

             if (face->use == 1)
             {
               /* try insertion of new node */

               for (k=1; k<=NEW_NODES; k++)
               {
                /* node inserted */

                check = Msh3DInsertNewNodesAdj (face, nnodes, k,
                          &alloc, &face_pop, dsp) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckValid( face, node_list, (*nnodes)-1,
                           -1, k, 1, *elems, nelems ) ;

                /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = (*nnodes) - 1 ;
               }
               else {
                (*nnodes)-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }

          }

          /* if no node was found even with the insertion of new nodes by
             procedures described above, return to main driver and write
             error code if this face is the first face that went two times
             to to the end of stack (bdry_try face represented by use equal
             to 2) or push this face into the end of stack and try next
             face. Else if a node was found, reset the first face that went
             to the end of stack (bdry_try face) and its use, considering
             that if this face has use equal to 0 then reset bdry_try face
             to no one face and continue trying as before, else if its use
             is equal to 1 then reset bdry_try face to current one and
             continue trying now inserting a new node always when needed */

          if ( node_indx == -1 ) {
           if (face->use == 2) {
             Msh3DBdryPush (face) ;
             if ( Msh3DCorrePoly (nnodes, nelems,
                                  elems, 0, 1, 0, dsp, pjmesh_use_box_test) ) break ;
             Msh3D_errno = Msh3D_INSERT_NO_VALID_NODES ;
             strcpy( Msh3D_err_msg,
              "Msh3DBdryContraction : Insertion of new nodes wasn't enough" ) ;
             return(0);
           }
           else {
             face->use++ ;
             face_pop++ ;
             Msh3DBdryPush (face) ;
             continue ;
           }
          }

          /* add the new element to the element list, update the
             boundary face list, and update the active node list. */

             Msh3DAddElem( nelems, elems, face, node_indx ) ;
             Msh3DAddFaces( face, node_list, node_indx, nelems ) ;
       }

       /* put back the faces that were saved onto a stack */

       for( i = 0; i < nface_star; i++ )
        bdry_face = Msh3DPushBdryFace( node_list, face_star[i*3+0],
                face_star[i*3+1], face_star[i*3+2], 0, 1 ) ;

       /* build the list of adjacent faces for all the nodes */

       Msh3DBdryReset() ;
       while ( face = Msh3DBdryNext() ) {
        for ( j=0 ; j<3 ; j++ ) {
         node = &node_list[face->verts[j]] ;
         adj_face = Msh3DAdjFaceAlloc() ;
         adj_face->next = node->faces ;
         adj_face->face = face ;
         node->faces = adj_face ;
         node_list[face->verts[j]].active_flag = 1 ;
        }
       }

       /* reset number of saves saved and get another polyhedron */

       nface_star = 0 ;
       nface_push = 0 ;
       break ;
     }
   }

/* PJMesh - deinvert faces of the remaining front */
	if (pjmesh_use_box_test)
	{
		PMsh3DInvertFaces();
	}
/* end PJMesh - deinvert faces of the remaining front */

   /* Print timing for this polyhedron. Only print if this is the
    * back-tracking phase, because for quality improvement the
    * timing is printed outside this function. */

#if DIA_BACK
    fprintf( stderr,"\t\t Back-tracking dele: \t Worked!!!\n");
    fprintf( stderr,"\t\t Deleted   elements: %8d ele.\n", diare);
#endif

#if DSP_TIME
   if (!shp) {
    tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
    fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *nelems);
    fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
    fprintf( stderr,"\n") ;
   }
#endif

	//added by markos
	face = bdry_stack ;
	//end added by markos
  }

  /* free allocated memory */

  Msh3DFree( face_node ) ;
  Msh3DFree( face_face ) ;
  Msh3DFree( face_star ) ;
  Msh3DFree( face_push ) ;

  /* if we get here all polyhedrons were corrected and the mesh
     was generated */

//if (shp) fprintf (stderr,"markos debug, improvement back tracking worked\n") ;
//else fprintf (stderr,"markos debug, real back tracking worked\n") ;

  return 1 ;
}

#if 0
static void Msh3DCrtnFDelV (
 Msh3DBdryFace visi,
 int           *elem,
 int           *nelem,
 int           corre )
{
 int            i, j, k, fac, vert, v0, v1, v2, f_verts[3], ind, node_indx ;
 int            found_node, found_face, ve, valid ;
 int            face_node[10000], nface_node = 0 ;
 double         metric ;
 Msh3DAdjFace   cur, del, *temp, save ;
 Msh3DBdryFace  face, face_to_delete, new_bdry ;
 Msh3DPolyFace  new_poly, cur_poly ;

 /* if it's second step here desconsider visi */

 if ( corre > (MSH3D_POLY_FACE_CORRE*2) )
  visi = 0 ;

 /* traverse the polyhedron structure to find all polyhedron nodes */

 Msh3DPolyReset () ;
 while (cur_poly = Msh3DPolyNext ()) {
  face = cur_poly->face ;
  for (i = 0; i < 3; i++) {
   ve = face->verts[i] ;
   found_node = 0 ;
   for (j = 0; j < nface_node; j++) {
    if (face_node[j] == ve) {
     found_node = 1 ;
     break ;
    }
   }
   if (!found_node) {
    face_node[nface_node++] = ve ;
   }
  }
 }

 /* rank all nodes based in its concativity */

 Msh3DHeapInit( nface_node ) ;
 for (i = 0; i < nface_node; i++) {
  node_indx = face_node[i] ;
  metric = 0.0 ;
  Msh3DPolyReset () ;
  while (cur_poly = Msh3DPolyNext ()) {
   face = cur_poly->face ;
   if ((face->verts[0] != node_indx) &&
       (face->verts[1] != node_indx) &&
       (face->verts[2] != node_indx)  ) {
    metric += Msh3DHeapSolidAngle( face, node_list, face->verts[0],
                face->verts[1], face->verts[2], node_indx ) ;
   }
  }
  Msh3DHeapInsert( (-1.0*metric), node_indx ) ;
 }

 /* find the most concave node in the polyhedron. If the most concave node
    is a boundary node, try another one because boundary nodes can't be
    deleted. Also allow only nodes that belongs to the given unvisible
    face. */

 valid = 0 ;
 while (!valid) {
  node_indx = Msh3DHeapExtract( &metric ) ;
  if (node_indx == -1) break ;
  else                 valid = 1 ;
  if (visi)
   if (!((node_indx == visi->verts[0]) ||
         (node_indx == visi->verts[1]) ||
         (node_indx == visi->verts[2])) )
    valid = 0 ;
  if (node_indx < nbdrynode)                 valid = 0 ;
  if (node_list[node_indx].active_flag == 0) valid = 0 ;
 }

 /* if no node was found, just return */

 if (node_indx < nbdrynode)                 return ;
 if (node_list[node_indx].active_flag == 0) return ;

 /* delete all faces adjacent to this node and update all nodes adjacency,
    the boundary stack and the polyhedron stack */

 for ( cur=node_list[node_indx].faces ; cur ; cur=cur->next ) {
  for ( i = 0 ; i < 3 ; i++ ) {
   vert = cur->face->verts[i] ;
   if ( vert == node_indx ) continue ;
   for ( del=node_list[vert].faces ; del ; del=del->next ) {
    if (( del->face->verts[0] == cur->face->verts[0] ) &&
        ( del->face->verts[1] == cur->face->verts[1] ) &&
        ( del->face->verts[2] == cur->face->verts[2] )  ) {
     face_to_delete = del->face ;
     for ( temp = &(node_list[vert].faces) ; *temp ;
           temp = &((*temp)->next) ) {
      if ( (*temp)->face == face_to_delete ) {
       save = *temp ;
       *temp = (*temp)->next ;
       Msh3DAdjFree( save ) ;
       break ;
      }
     }
     break ;
    }
   }
   if ( !node_list[vert].faces )
    node_list[vert].active_flag = 0 ;
  }
  Msh3DPolyDelete( cur->face ) ;
  Msh3DBdryDelete( cur->face ) ;
 }

 /* delete the elements related with this node from the element list,
    updating the boundary and polyhedron stacks */

 for ( i = 0 ; i < *nelem; i++ ) {

  /* get the element */

  ind = i*4 ;

  /* the element should be deleted only if contains the node */

  if (( elem[ind+0]==node_indx ) ||
      ( elem[ind+1]==node_indx ) ||
      ( elem[ind+2]==node_indx ) ||
      ( elem[ind+3]==node_indx )  ) {

   /* update the stacks for the element. There's only one face in the
      element that should be considered, the face that not contains
      the node */

   for ( j = 0; j < 4; j++ ) {

    /*  get the element face that not contains the node */

    if ( elem[ind+j] == node_indx ) {
     if ( j == 0 ) v0 = elem[ind+1], v1 = elem[ind+3], v2 = elem[ind+2] ;
     if ( j == 1 ) v0 = elem[ind+2], v1 = elem[ind+3], v2 = elem[ind+0] ;
     if ( j == 2 ) v0 = elem[ind+3], v1 = elem[ind+1], v2 = elem[ind+0] ;
     if ( j == 3 ) v0 = elem[ind+0], v1 = elem[ind+1], v2 = elem[ind+2] ;

     /* verify if the face is already in the polyhedron stack */

     Msh3DPolyReset() ;
     while (cur_poly = Msh3DPolyNext() ) {
      fac = 0 ;
      face = cur_poly->face ;
      for ( k = 0 ; k < 3; k++ ) {
       if ( (face->verts[k] == v0) ||
            (face->verts[k] == v1) ||
            (face->verts[k] == v2) )
        fac++ ;
      }
      if (fac == 3) break ;
     }

     /* if the face is already in the boundary stack, it should be deleted
        from both boundary and polyhedron stacks. The only case that this
        can't be done is when the face belongs to the original boundary.
        Else if the face isn't in the boundary stack, it should be included
        in both boundary and polyhedron stacks. */

     if ( fac == 3 )
     {
      for ( k = 0; k < 3; k++ )
       f_verts[k] = face->verts[k] ;
      if ( !Msh3DBoundaryFace (face, f_verts) ) {
       for ( k=0 ; k<3 ; k++ ) {
        for ( temp = &(node_list[face->verts[k]].faces) ; *temp ;
              temp = &((*temp)->next) ) {
         if ( (*temp)->face == face ) {
          save = *temp ;
          *temp = (*temp)->next ;
          Msh3DAdjFree( save ) ;
          break ;
         }
        }
        if ( !node_list[face->verts[k]].faces )
         node_list[face->verts[k]].active_flag = 0 ;
       }
       Msh3DPolyDelete( face ) ;
       Msh3DBdryDelete( face ) ;
      }
     }
     else
     {
      f_verts[0] = v0 ;
      f_verts[1] = v1 ;
      f_verts[2] = v2 ;
      if ( !Msh3DVerifFace (f_verts, &face) ) {
       new_bdry = Msh3DPushBdryFace( node_list, v0, v1, v2, 2, 1 ) ;
       new_poly = Msh3DPushPolyFace( new_bdry ) ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v0].faces ;
       node_list[v0].faces = cur ;
       node_list[v0].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v1].faces ;
       node_list[v1].faces = cur ;
       node_list[v1].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v2].faces ;
       node_list[v2].faces = cur ;
       node_list[v2].active_flag = 1 ;
      }
      else {
       new_poly = Msh3DPushPolyFace( face ) ;
      }
     }
    }
   }

   /* delete the element from the element list */

   elem[ind+0] = -1 ;
   elem[ind+1] = -1 ;
   elem[ind+2] = -1 ;
   elem[ind+3] = -1 ;
  }
 }

 /* update active_flag of the deleted node */

 node_list[node_indx].active_flag = 0 ;

 /* just to check, verify if all faces of boundary are in the polyhedron */

 nface_node = 0 ;
 Msh3DPolyReset () ;
 while (cur_poly = Msh3DPolyNext ()) {
  face = cur_poly->face ;
  for (i = 0; i < 3; i++) {
   ve = face->verts[i] ;
   found_node = 0 ;
   for (j = 0; j < nface_node; j++) {
    if (face_node[j] == ve) {
     found_node = 1 ;
     break ;
    }
   }
   if (!found_node) {
    face_node[nface_node++] = ve ;
   }
  }
 }

 Msh3DBdryReset () ;
 while (face = Msh3DBdryNext ()) {
  fac = 0 ;
  for (i = 0; i < 3; i++) {
   for (j = 0; j < nface_node; j++) {
    if (face_node[j] == face->verts[i]) {
     fac++ ;
     break ;
    }
   }
  }
  if (fac == 3) {
   found_face = 0 ;
   Msh3DPolyReset () ;
   while (cur_poly = Msh3DPolyNext ()) {
    if (cur_poly->face == face) {
     found_face = 1 ;
     break ;
    }
   }
   if (!found_face) {
    v0 = face->verts[0] ;
    v1 = face->verts[1] ;
    v2 = face->verts[2] ;
    new_poly = Msh3DPushPolyFace( face ) ;
   }
  }
 }

}
#endif

static void Msh3DCrtnFDelF (
 Msh3DBdryFace visi,
 int           *elem,
 int           *nelem,
 int           corre,
 int           shp,
 int           pjmesh_use_box_test )
{
 int            i, j, k, vert, f_verts[3], v0, v1, v2, v[3], vd[3], ind ;
 int            ve, elm, fac, recon, w[3], found ;
 int            found_node, found_face, face_num, check ;
 int            nface_node = 0 ;
 int            nface_face = 0 ;
 Msh3DAdjFace   cur, *temp, save ;
 Msh3DBdryFace  face, face_to_delete, new_bdry, next ;
 Msh3DPolyFace  new_poly, cur_poly, cur_next ;
 Msh3DBdryFace  *face_face ;
 Msh3DBdryFace  face_dele[1000] ;
 int            *face_node ;

 /* allocate memory for facec vector */

 face_node = (int *)Msh3DMalloc( (10*MSH3D_ELEM_QUANTUM)*sizeof(int) ) ;
 face_face = (Msh3DBdryFace *)Msh3DMalloc((10*MSH3D_ELEM_QUANTUM)*sizeof(Msh3DBdryFace) ) ;

 /* if there's no face to be deleted, take the poly_stack then */

 if (!visi)
  visi = poly_stack->face ;

/*//debug
	printf("passed visi face %d %d %d, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));

	if ((visi->verts[0] == 447) && (visi->verts[1] == 443) && (visi->verts[2] == 472))
	{
		printf("found visi face %d %d %d\n", visi->verts[0], visi->verts[1], visi->verts[2]);
	}
//endebug*/

#if OLD_NEAR
 /* get the vertexs of the unvisible face */
 for ( i = 0; i < 3; i++ )
   v[i] = visi->verts[i] ;
#else
 /* get the vertexs of the unvisible face */
 for ( i = 0; i < 3; i++ )
   v[i] = visi->verts[i] ;
 /* check if the deletion will bring the polyhedron dangerously near to the
  * original boundary. If so, pick another intersected face, if exists */
 if (!shp) {
  if (Msh3DCreationFace( v, elem, nelem, pjmesh_use_box_test )) {
   /* if we get here the unvisible face will insert faces from the
    * original boundary. Try to delete a different face instead */
   Msh3DPolyReset () ;
   while (cur_poly = Msh3DPolyNext ()) {
    face = cur_poly->face ;
    if (face->use <= 0) continue ;   /* don't pick a not intersected face */
    if (face == visi)   continue ;   /* don't pick unvisible face */
    for ( i = 0; i < 3; i++ )
     w[i] = face->verts[i] ;
    if (Msh3DBoundaryFace(face, w))             continue ; /* no boundary */
    if (Msh3DNoElemFace (face, w, elem, nelem)) continue ; /* no elements */
    if (!Msh3DCreationFace( w, elem, nelem, pjmesh_use_box_test )) {
     /* if we get here, this face is also unvisible and it it not going
      * to insert faces from the original boundary. */
     visi = face ;
     for ( i = 0; i < 3; i++ )
      v[i] = visi->verts[i] ;
#if DIA_NEAR
     fprintf( stderr,"Face alternative is going to be deleted:\n" ) ;
     fprintf( stderr,"Face (internally) = %d\t%d\t%d\n",v[0], v[1], v[2]) ;
     fprintf( stderr,"Face (actually)   = %d\t%d\t%d\n",
 		                         node_list[v[0]].real_id,
		                         node_list[v[1]].real_id,
					 node_list[v[2]].real_id) ;
     fprintf( stderr,"Face intersections = %d\n", visi->use ) ;
#endif
     break ;
    }
   }
  }
 }
#endif

#if OLD_RECO
 /* basically the test that should be done here is to check if the unvisible
    face has no element associated with it. This could happen if the
    unvisible face  is a boundary face, in those three situations: a) it
    belongs to the original boundary; b) it is a crack face, which also
    belongs to the boundary or c) it means that another polyhedron in
    the neighborhood has reached. In situations a) and b) the unvisible face
    can not be deleted. So, try to delete another one instead. In situation
    c) we have two options: a) delete another one instead or b) join the
    two polyhedra and delete the repeated faces */

 while ( Msh3DNoElemFace (visi, v, elem, nelem))
 {
#if DIA_RECO
  if (!shp) {
   fprintf( stderr,"Face with no element associated was reached:\n" ) ;
   fprintf( stderr,"Face (internally) = %d\t%d\t%d\n",v[0], v[1], v[2]) ;
   fprintf( stderr,"Face (actually)   = %d\t%d\t%d\n",node_list[v[0]].real_id,
		                                      node_list[v[1]].real_id,
					              node_list[v[2]].real_id) ;
   Mesh3DDiaDelf (elem, nelem) ;
  }
#endif
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   for ( i = 0; i < 3; i++ )
    v[i] = face->verts[i] ;
   if( !Msh3DNoElemFace (face, v, elem, nelem) )
   {
    if (!Msh3DBoundaryFace (face, v))
    {
     visi = face ;
     break ;
    }
   }
  }
 }
#else
 /* check if the face to be deleted has any element associated with it.
  * If it doesn't it can mean two cases: a) the face belongs to another
  * polyhedron that will be treated in the future and the current
  * polyhedron reached this one; b) the face belongs to the original
  * boundary. So, if it is case a), we reconstruct the polyhedron stack
  * to include now also the other polyhedron associated with the face to
  * be deleted, and if it's case b) we have to pick another face to delete.*/

/*//debug
	printf("visi face %d %d %d before while, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));

	if ((visi->verts[0] == 447) && (visi->verts[1] == 443) && (visi->verts[2] == 472))
	{
		printf("found visi face %d %d %d\n", visi->verts[0], visi->verts[1], visi->verts[2]);
	}
//endebug*/

 while (Msh3DNoElemFace (visi, v, elem, nelem)
//added by markos
		&& ((!pjmesh_use_box_test) || ((shp) ? PMsh3DIsInvertedFace(visi) : 0))
//end added by markos
		)
 {
#if DIA_RECO
  if (!shp) {
   fprintf( stderr,"Face with no element associated was reached:\n" ) ;
   fprintf( stderr,"Face (internally) = %d\t%d\t%d\n",v[0], v[1], v[2]) ;
   fprintf( stderr,"Face (actually)   = %d\t%d\t%d\n",node_list[v[0]].real_id,
                                                      node_list[v[1]].real_id,
                                                      node_list[v[2]].real_id) ;
   Mesh3DDiaDelf (elem, nelem) ;
  }
#endif

/*//debug
	printf("visi face %d %d %d within while, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));
//endebug*/

  /* Delete the polyhedron and reconstruct it. Use reconstruct polyhedron
   * because elements can be repeated and we can have pathological cases. */
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   Msh3DPolyDelete( face ) ;
  }
  poly_stack = poly_tail = NULL ;
  face = visi ;
  Msh3DRecoPoly (face, face_face, face_node, &nface_face, &nface_node, 1, pjmesh_use_box_test) ;

/*//debug
	printf("visi face %d %d %d after Msh3DRecoPoly, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));
//endebug*/
#if DIA_RECO
  fprintf( stderr,"Polyhedron reconstructed\n") ;
  Mesh3DDiaDelf (elem, nelem) ;
#endif
  /* Pick another unvisible face because the current one doesn't have
   * any element associated with or just delete the repeated faces and
   * return, to get a better unvivisible face. */
#if OLD_FIXE
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   for ( i = 0; i < 3; i++ )
    v[i] = face->verts[i] ;
   if( !Msh3DNoElemFace (face, v, elem, nelem) )
   {
    if (!Msh3DBoundaryFace (face, v))
    {
     visi = face ;
     break ;
    }
   }
  }
#else
  if (!shp) {
   Msh3DDelRepeatedFace ( pjmesh_use_box_test ) ;
   Msh3DFree( face_node ) ;
   Msh3DFree( face_face ) ;
   return ;
  } else {
   Msh3DPolyReset() ;

/*//debug
	printf("visi face %d %d %d within while before while, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));
//endebug*/

   while ( cur_poly = Msh3DPolyNext() ) {
    face = cur_poly->face ;
    for ( i = 0; i < 3; i++ )
     v[i] = face->verts[i] ;
    if( !Msh3DNoElemFace (face, v, elem, nelem)
		//added by markos
		&& ((!pjmesh_use_box_test) || (!PMsh3DIsInvertedFace(face)))
		//end added by markos
	)
    {
     if ((!Msh3DBoundaryFace (face, v))
		//added by markos
		&& ((!pjmesh_use_box_test) || ((!PMsh3DIsRemainingFrontFace(v[0], v[1], v[2])) && (!PMsh3DIsInvertedFace(face))))
		//end added by markos
		)
     {
      visi = face ;

/*//debug
	printf("visi face %d %d %d within while within while, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));
//endebug*/

      break ;
     }
    }
   }
  }
#endif
 }
#endif

/*//debug
	printf("chosen visi face %d %d %d, inverted = %d, remaining = %d\n", visi->verts[0], visi->verts[1], visi->verts[2], PMsh3DIsInvertedFace(visi), PMsh3DIsRemainingFrontFace(visi->verts[0], visi->verts[1], visi->verts[2]));
//endebug*/

 /* if you don't get any unvisible face, just return */

 if (visi == 0) {
  Msh3DFree( face_node ) ;
  Msh3DFree( face_face ) ;
  return ;
 }

 /* delete unvisible face and update all nodes adjacency, the boundary
    stack and the polyhedron stack */

 face_to_delete = visi ;
 for ( j = 0; j < 3; j++ ) {
  vert = v[j] ;
  for ( temp = &(node_list[vert].faces) ; *temp ;
        temp = &((*temp)->next) ) {
   if ( (*temp)->face == face_to_delete ) {
    save = *temp ;
    *temp = (*temp)->next ;
    Msh3DAdjFree( save ) ;
    break ;
   }
  }
  if ( !node_list[vert].faces )
   node_list[vert].active_flag = 0 ;
 }
 Msh3DPolyDelete( visi ) ;
 Msh3DBdryDelete( visi ) ;

 /* delete the elements related with the unvisible face from the element
    list, updating the boundary and polyhedron stacks */

#if OLD_ELEM
 for ( i = 0 ; i < *nelem; i++ ) {

  /* get the element */

  ind = i*4 ;

  /* verify if the face belongs to the element */

  elm = 0 ;
  for ( k = 0; k < 3 ; k++ )
  {
   if ( (v[k]==elem[ind+0]) || (v[k]==elem[ind+1]) ||
        (v[k]==elem[ind+2]) || (v[k]==elem[ind+3]) )
     elm++ ;
  }

  /* update the stacks for the element if the face belongs to it. There's
     three faces that should be considered, there is, the faces different
     from the unvisible face */

  if ( elm == 3 )       /* given face belongs to the element */
  {
   /*  get the element faces that are different from the unvisible face */

   for ( j = 0; j < 4; j++ )
   {
    if ( j == 0 ) v0 = elem[ind+1], v1 = elem[ind+3], v2 = elem[ind+2] ;
    if ( j == 1 ) v0 = elem[ind+2], v1 = elem[ind+3], v2 = elem[ind+0] ;
    if ( j == 2 ) v0 = elem[ind+3], v1 = elem[ind+1], v2 = elem[ind+0] ;
    if ( j == 3 ) v0 = elem[ind+0], v1 = elem[ind+1], v2 = elem[ind+2] ;

    /* verify if element face is different from unvisible face */

    fac = 0 ;
    for ( k = 0 ; k < 3 ; k++ ) {
     if ( (v[k] == v0) || (v[k] == v1) || (v[k] == v2) )
      fac++ ;
    }

    /* update the stacks for the element face if it's different from the
       unvisible face */

    if ( fac != 3 )
    {
     /* verify if the face is already in the polyhedron stack */

     Msh3DPolyReset() ;
     while (cur_poly = Msh3DPolyNext() ) {
      fac = 0 ;
      face = cur_poly->face ;
      for ( k = 0 ; k < 3; k++ ) {
       if ( (face->verts[k] == v0) ||
            (face->verts[k] == v1) ||
            (face->verts[k] == v2) )
        fac++ ;
      }
      if (fac == 3) break ;
     }

     /* if the face is already in the polyhedron stack, it should be deleted
        from both boundary and polyhedron stacks. The only case that this
        can't be done is when the face belongs to the original boundary.
        Else if the face isn't in the polyhedron stack, it should be included
        in both boundary and polyhedron stacks. */

     if ( fac == 3 )
     {
      for ( k = 0; k < 3; k++ )
       f_verts[k] = face->verts[k] ;
      if ( !Msh3DBoundaryFace (face, f_verts)
		//added by markos
		&& ((!pjmesh_use_box_test) || (!PMsh3DIsInvertedFace(face)))
		//end added by markos
		) {
       for ( k=0 ; k<3 ; k++ ) {
        for ( temp = &(node_list[face->verts[k]].faces) ; *temp ;
              temp = &((*temp)->next) ) {
         if ( (*temp)->face == face ) {
          save = *temp ;
          *temp = (*temp)->next ;
          Msh3DAdjFree( save ) ;
          break ;
         }
        }
        if ( !node_list[face->verts[k]].faces )
         node_list[face->verts[k]].active_flag = 0 ;
       }
       Msh3DPolyDelete( face ) ;
       Msh3DBdryDelete( face ) ;
      }
      else {
       fprintf( stderr, "face   boundary  = \t%d\t%d\t%d\n", v0, v1, v2) ;
      }
     }
     else
     {
      f_verts[0] = v0 ;
      f_verts[1] = v1 ;
      f_verts[2] = v2 ;

/* PJMesh - if face belongs to the remaining front, invert it */
		if (pjmesh_use_box_test)
		{
			new_bdry = PMsh3DRemainingFrontFace(v0, v1, v2);
		}

		if ((pjmesh_use_box_test) && (new_bdry))
		{
			if (!PMsh3DInvertFace(new_bdry))
			{
				fprintf(stderr, "face inverted twice and removed from list of inverted = %d %d %d\n", v0, v1, v2);
			}

			new_poly = Msh3DPushPolyFace( new_bdry ) ;
		}
		else
/* end PJMesh - if face belongs to the remaining front, invert it */
      if ( 1 /* !Msh3DVerifFace (f_verts, &face) */) {
       new_bdry = Msh3DPushBdryFace( node_list, v0, v1, v2, 2, 1 ) ;
       new_poly = Msh3DPushPolyFace( new_bdry ) ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v0].faces ;
       node_list[v0].faces = cur ;
       node_list[v0].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v1].faces ;
       node_list[v1].faces = cur ;
       node_list[v1].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v2].faces ;
       node_list[v2].faces = cur ;
       node_list[v2].active_flag = 1 ;
      }
      else {
       new_poly = Msh3DPushPolyFace( face ) ;
      }
     }
    }
   }


   /* save the deleted elements if we're trying to improve the shape (where
      we allow Msh3D_DELETE_ELEM steps to be performed) */

   if( shp )
   {
    if( num_elem_created < Msh3D_DELETE_ELEM )
    {
     adj_elem_dele[num_elem_deleted].elem = ind / 4;
     adj_elem_dele[num_elem_deleted].verts[0] = elem[ind+0] ;
     adj_elem_dele[num_elem_deleted].verts[1] = elem[ind+1] ;
     adj_elem_dele[num_elem_deleted].verts[2] = elem[ind+2] ;
     adj_elem_dele[num_elem_deleted].verts[3] = elem[ind+3] ;
     num_elem_deleted++ ;
     num_elem_created++ ;
    }
   }

   /* delete the element from the element list */

   elem[ind+0] = -1 ;
   elem[ind+1] = -1 ;
   elem[ind+2] = -1 ;
   elem[ind+3] = -1 ;
  }
 }
#else
 for ( i = 0 ; i < 2; i++ ) {

  /* get the element */

  if (face_to_delete->rgn[i] == -1) continue ;
  ind = face_to_delete->rgn[i]*4 ;

  /* verify if the face belongs to the element */

  elm = 0 ;
  for ( k = 0; k < 3 ; k++ )
  {
   if ( (v[k]==elem[ind+0]) || (v[k]==elem[ind+1]) ||
        (v[k]==elem[ind+2]) || (v[k]==elem[ind+3]) )
     elm++ ;
  }

  /* update the stacks for the element if the face belongs to it. There's
     three faces that should be considered, there is, the faces different
     from the unvisible face */

  if ( elm == 3 )       /* given face belongs to the element */
  {
   /*  get the element faces that are different from the unvisible face */

   for ( j = 0; j < 4; j++ )
   {
    if ( j == 0 ) v0 = elem[ind+1], v1 = elem[ind+3], v2 = elem[ind+2] ;
    if ( j == 1 ) v0 = elem[ind+2], v1 = elem[ind+3], v2 = elem[ind+0] ;
    if ( j == 2 ) v0 = elem[ind+3], v1 = elem[ind+1], v2 = elem[ind+0] ;
    if ( j == 3 ) v0 = elem[ind+0], v1 = elem[ind+1], v2 = elem[ind+2] ;

    /* verify if element face is different from unvisible face */

    fac = 0 ;
    for ( k = 0 ; k < 3 ; k++ ) {
     if ( (v[k] == v0) || (v[k] == v1) || (v[k] == v2) )
      fac++ ;
    }

    /* update the stacks for the element face if it's different from the
       unvisible face */

    if ( fac != 3 )
    {
     /* verify if the face is already in the polyhedron stack */

     Msh3DPolyReset() ;
     while (cur_poly = Msh3DPolyNext() ) {
      fac = 0 ;
      face = cur_poly->face ;
      for ( k = 0 ; k < 3; k++ ) {
       if ( (face->verts[k] == v0) ||
            (face->verts[k] == v1) ||
            (face->verts[k] == v2) )
        fac++ ;
      }
      if (fac == 3) break ;
     }

     /* if the face is already in the polyhedron stack, it should be deleted
        from both boundary and polyhedron stacks. The only case that this
        can't be done is when the face belongs to the original boundary.
        Else if the face isn't in the polyhedron stack, it should be included
        in both boundary and polyhedron stacks. */

     if ( fac == 3 )
     {
      for ( k = 0; k < 3; k++ )
       f_verts[k] = face->verts[k] ;
      if ( !Msh3DBoundaryFace (face, f_verts) ) {
       for ( k=0 ; k<3 ; k++ ) {
        for ( temp = &(node_list[face->verts[k]].faces) ; *temp ;
              temp = &((*temp)->next) ) {
         if ( (*temp)->face == face ) {
          save = *temp ;
          *temp = (*temp)->next ;
          Msh3DAdjFree( save ) ;
          break ;
         }
        }
        if ( !node_list[face->verts[k]].faces )
         node_list[face->verts[k]].active_flag = 0 ;
       }
       if (face->rgn[0] == face_to_delete->rgn[i]) face->rgn[0] = -1 ;
       if (face->rgn[1] == face_to_delete->rgn[i]) face->rgn[1] = -1 ;
#if DIA_ELEM
       if ((face->rgn[0]!=face_to_delete->rgn[i]) &&
           (face->rgn[1]!=face_to_delete->rgn[i])) {
        fprintf (stderr, "Element not found for face:\n") ;
        fprintf (stderr, "Face (internally) = %d %d %d\n",
                                              face->verts[0],
                                              face->verts[1],
                                              face->verts[2]) ;
        fprintf (stderr, "Face (actually)   = %d %d %d\n",
                                     node_list[face->verts[0]].real_id,
                                     node_list[face->verts[1]].real_id,
                                     node_list[face->verts[2]].real_id) ;
        fprintf (stderr, "Element = %d\n", face_to_delete->rgn[i]) ;
       }
#endif
       Msh3DPolyDelete( face ) ;
       Msh3DBdryDelete( face ) ;
      }
      else {
       fprintf( stderr, "face   boundary  = \t%d\t%d\t%d\n", v0, v1, v2) ;
      }
     }
     else
     {
      f_verts[0] = v0 ;
      f_verts[1] = v1 ;
      f_verts[2] = v2 ;
      if ( 1 /* !Msh3DVerifFace (f_verts, &face) */ ) {
       new_bdry = Msh3DPushBdryFace( node_list, v0, v1, v2, 2, 1 ) ;
       new_poly = Msh3DPushPolyFace( new_bdry ) ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v0].faces ;
       node_list[v0].faces = cur ;
       node_list[v0].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v1].faces ;
       node_list[v1].faces = cur ;
       node_list[v1].active_flag = 1 ;

       cur = Msh3DAdjFaceAlloc() ;
       cur->face = new_bdry ;
       cur->next = node_list[v2].faces ;
       node_list[v2].faces = cur ;
       node_list[v2].active_flag = 1 ;

       /* look for the element associated to this face */

       for (m = 0; m < (*nelem); m++) {
        nind = m*4 ;
        elm = 0 ;
        for (k = 0; k < 3; k++) {
         if ( (new_bdry->verts[0] == elem[nind+0]) ||
              (new_bdry->verts[1] == elem[nind+1]) ||
              (new_bdry->verts[2] == elem[nind+2]) ||
              (new_bdry->verts[3] == elem[nind+3]) )
          elm++ ;
        }
	if (elm == 3) {
	 new_bdry->rgn[0] = m ;
	 new_bdry->rgn[1] = -1 ;
	 break ;
	}
       }

      }
      else {
       new_poly = Msh3DPushPolyFace( face ) ;
      }
     }
    }
   }


   /* save the deleted elements if we're trying to improve the shape (where
      we allow Msh3D_DELETE_ELEM steps to be performed) */

   if( shp )
   {
    if( num_elem_created < Msh3D_DELETE_ELEM )
    {
     adj_elem_dele[num_elem_deleted].elem = ind / 4;
     adj_elem_dele[num_elem_deleted].verts[0] = elem[ind+0] ;
     adj_elem_dele[num_elem_deleted].verts[1] = elem[ind+1] ;
     adj_elem_dele[num_elem_deleted].verts[2] = elem[ind+2] ;
     adj_elem_dele[num_elem_deleted].verts[3] = elem[ind+3] ;
     num_elem_deleted++ ;
     num_elem_created++ ;
    }
   }

   /* delete the element from the element list */

   elem[ind+0] = -1 ;
   elem[ind+1] = -1 ;
   elem[ind+2] = -1 ;
   elem[ind+3] = -1 ;
  }
 }
#endif

 /* update active_flag of the nodes of the unvisible face. Also,
  * check if you artificially included faces with no element
  * associaated in the polyhedron. This can happens when one
  * polyhedron touches another one in the vicinity. Faces can
  * not have associated elements only if they are in the
  * original boundary. Otherwise they should be deleted from
  * the boundary and the polyhedron should be reconstructed. */

#if OLD_FIXE
 /* update active_flag of nodes for unvisible face */
 for ( i = 0 ; i < 3 ; i++ ) {
  if ( !node_list[v[i]].faces )
   node_list[v[i]].active_flag = 0 ;
 }
#else
 /* delete faces that are repeated in the polyhedron. These faces
  * were introduced when the current polyhedron reached another one
  * in its vicinity and a same face was introduced, but reversed.
  * All of this process should not be done in the deleting phase. */
 if (!shp) {
  Msh3DDelRepeatedFace ( pjmesh_use_box_test ) ;
#if DIA_FIXE
  fprintf( stderr,"Polyhedron fixed\n") ;
  Mesh3DDiaDelf (elem, nelem) ;
#endif
 }
 /* update active_flag of nodes for unvisible face */
 for ( i = 0 ; i < 3 ; i++ ) {
  if ( !node_list[v[i]].faces )
   node_list[v[i]].active_flag = 0 ;
 }
#endif

 /* free allocated memory */

 Msh3DFree( face_node ) ;
 Msh3DFree( face_face ) ;
}

/* -------------------------------------------------------------------
** Msh3DShapePoly - these routines implement correct elements procedure.
*/

static int Msh3DShapePoly (
  int                *nnodes,
  int                *nelems,
  int                **elems,
  int                *shape,
  int                *step,
  int                gdb,
  int                dsp,
  int                pjmesh_use_box_test)
{
  Msh3DBdryFace     face, cur, next, cur_face ;
  Msh3DBdryNode     node ;
  Msh3DAdjFace      adj_face ;
  Msh3DAdjIniFace   adj_ini_face ;
  Msh3DId          *aux ;
  double            cand_vec[3], dot, tol ;
  double            len, size = 1.0e+7 ;
  double            metric, next_metric, delta ;
  double           *nodes, **vec_nodes ;
  int              *faces, **vec_faces ;
  int               num_face, num_node, num_elem, num_bdry ;
  int               i, j, k, l, sliver, first ;
  int               check, valid, corre, alloc, trials, face_pop = 0 ;
  int               node_indx, next_indx, star_poly = 1 ;
  int               face_done = 0, face_number = 10 ;
  int               face_draw = 0, face_try = 0 ;
  int               num_org_nodes ;
  int               num_org_faces ;
  double            original_nodes[1000][3] ;
  double           *generated_nodes ;
  int               original_faces[1000][3] ;
  int               num_gen_nodes = 0;
  int               v1i, v2i, v1, v2, found, exist, pok, shp = 1 ;
  int               cur_nelems ;
  //added by markos
  int count = 0;
  //end added by markos

  /* update shape and step for sliver shape polyhedron procedure */

  *shape  = 0 ;
  *step  += 1 ;
  if( *step > Msh3D_DELETE_STEP ) {
   *shape = 0 ;
   return 1 ;
  }

  /* get number of elements and number of boundary nodes at this point */

  num_elem = (*nelems) ;
  num_bdry = (*nnodes) ;

  /* improve the mesh by sliver shape polyhedron procedure */

  for( l = 0; l < num_elem; l++ ) {

    /* find the polyhedron for sliver element */
    //PMsh3DPrintBdryList("\nbefore Msh3DElemPoly\n", NULL, 0, 1, 1);

    sliver = Msh3DElemPoly( l, nelems, *elems, &num_org_faces,
                            original_faces, *step ) ;
    if( !sliver ) continue ;

    //PMsh3DPrintBdryList("\nafter Msh3DElemPoly\n", NULL, 0, 1, 1);

//fprintf (stderr,"markos debug, back tracking, elem = %d will be improved by back tracking\n", l) ;

    /* init timing variables */

#if DSP_TIME
    init_time = clock( );
    cur_nelems = *nelems ;
#endif

    /* update shape for sliver shape polyhedron procedure */

    *shape = 1 ;

    /* release list of faces already made */

    Msh3DTestFreeAll() ;

    /* push all the boundary face descriptions onto a stack  */

#if OLD_DELE
    push_version = SMALL_VERSION ;
#else
    push_version = ADVAN_VERSION ;
    bdry_use1 = 0 ;
    bdry_use2 = 0 ;
    bdry_useNeg = 0 ;
#endif

    //commented by markos
    //for ( i=0 ; i<num_org_faces ; i++ )
    //    face = Msh3DPushBdryFace( node_list,
    //              original_faces[i][0],original_faces[i][1],
    //              original_faces[i][2], 0, 0 ) ;
    //end commented by markos
    //added by markos
    if (pjmesh_use_box_test)
    {
		for ( i=0 ; i<num_org_faces ; i++ )
		{
			face = bdry_stack;

			while (face)
			{
				count = 0;

				for (j = 0; j < 3; j++)
				{
					for (k = 0; k < 3; k++)
					{
						if (original_faces[i][j] == face->verts[k])
						{
							count++;

							break;
						}
					}
				}

				if (count == 3)
				{
					break;
				}

				face = face->next;
			}

			if (face)
			{
				//will be inverted when the polyhedron is built
				//PMsh3DInvertFace(face);
			}
			else
			{
				face = Msh3DPushBdryFace( node_list,
					  original_faces[i][0],original_faces[i][1],
					  original_faces[i][2], 0, 0 ) ;
			}
		}
    }
    else
    {
		for ( i=0 ; i<num_org_faces ; i++ )
			face = Msh3DPushBdryFace( node_list,
					  original_faces[i][0],original_faces[i][1],
					  original_faces[i][2], 0, 0 ) ;
    }
	//end added by markos

	//PMsh3DPrintBdryList("\nafter Msh3DPushBdryFace\n", NULL, 0, 1, 1);

#if OLD_ELEM
    Msh3DBdryReset () ;
    while (cur_face = Msh3DBdryNext ()) {
    	//added by markos
    	if ((pjmesh_use_box_test) && (cur_face->use == -1)) continue;
    	//end added by markos
     cur_face->rgn[0] = -1 ;
     cur_face->rgn[1] = -1 ;
    }
#else
    for (m = 0; m < (*nelems); m++) {
     ind = m*4 ;
     Msh3DBdryReset () ;
     while (cur_face = Msh3DBdryNext ()) {
      elm = 0 ;
      for (n = 0; n < 3; n++) {
       if ( (cur_face->verts[n]==(*elems)[ind+0]) ||
            (cur_face->verts[n]==(*elems)[ind+1]) ||
            (cur_face->verts[n]==(*elems)[ind+2]) ||
            (cur_face->verts[n]==(*elems)[ind+3]) )
        elm++ ;
      }
      if (elm == 3) {
       cur_face->rgn[0] =  m ;
       cur_face->rgn[1] = -1 ;
      }
     }
    }
#endif

	//PMsh3DPrintBdryList("\nafter 1st while\n", NULL, 0, 1, 1);

    push_version = ADVAN_VERSION ;

    /* build the list of adjacent faces for all the boundary nodes.
     * Also find the smallest edge on the model to define tolerance. */

    Msh3DBdryReset() ;
    while ( face = Msh3DBdryNext() ) {
		//added by markos
    	if ((pjmesh_use_box_test) && (face->use == -1)) continue;
    	//end added by markos
      for ( j=0 ; j<3 ; j++ ) {
          /* build adjacency lists */
          node = &node_list[face->verts[j]] ;
          adj_face = Msh3DAdjFaceAlloc() ;
          adj_face->next = node->faces ;
          adj_face->face = face ;
          node->faces = adj_face ;
          node_list[face->verts[j]].active_flag = 1 ;
	  /* find the smallest edge on the model */
          cand_vec[0] = node_list[face->verts[j]].coord[0] -
                        node_list[face->verts[(j+1)%3]].coord[0] ;
          cand_vec[1] = node_list[face->verts[j]].coord[1] -
                        node_list[face->verts[(j+1)%3]].coord[1] ;
          cand_vec[2] = node_list[face->verts[j]].coord[2] -
                        node_list[face->verts[(j+1)%3]].coord[2] ;
          len = sqrt ( cand_vec[0] * cand_vec[0] +
                       cand_vec[1] * cand_vec[1] +
                       cand_vec[2] * cand_vec[2] ) ;
          if( len < size ) size = len ;
      }
    }

    //PMsh3DPrintBdryList("\nafter 2nd while\n", NULL, 0, 1, 1);

    /* define tolerance based on the smallest edge on the model */

#if OLD_TOLE
    tol_inters = TOL_INTERS * size ;
#else
    tol_inters = 1.0e-08 * size ;
#endif

    /* here we have two options for the shape polyhedron procedure:
       the first  one is to use the correct polyhedron   procedure  and
       the second one is to use the boundary contraction procedure. The
       first one is currently being used because gives better results. */

    star_poly = Msh3DVisiPoly (nnodes, nelems, elems, *step, pjmesh_use_box_test) ;

    //PMsh3DPrintBdryList("\nafter Msh3DVisiPoly\n", NULL, 0, 1, 1);

    if( star_poly ) {      /* correct polyhedron procedure */

      /* initialize gdb package for shape polyhedron procedure */

#if DSP_MESH
      if ( FDEBUG && gdb ) {
        GdbIni ("Mesh3d Generator - Deleting Technique");
        GdbDrawBdry();
        gdb = 0;
      }
      else if (dsp) {
        GdbFit ( ) ;
	GdbClear();
	GdbDrawBdry();
      }
#endif

      /* use the correct polyhedron procedure for the given polyhedron */

      if( *step > 5 ) pok = 0 ;
      else            pok = 1 ;

      //PMsh3DPrintBdryList("\nbefore Msh3DCorrePoly\n", NULL, 0, 1, 1);

      if ( !Msh3DCorrePoly (nnodes, nelems,
                           elems, 0, pok, shp, dsp, pjmesh_use_box_test) ) {
       Msh3D_errno = Msh3D_INSERT_NO_VALID_NODES ;
       strcpy( Msh3D_err_msg,
        "Msh3DBdryContraction : Insertion of new nodes wasn't enough" ) ;
       return 0 ;
      }

      //PMsh3DPrintBdryList("\nafter Msh3DCorrePoly\n", NULL, 0, 1, 1);
     }
     else {                /* boundary contraction procedure */

      /* build octree for the polyhedron */

      num_face = num_org_faces;
      num_node = num_org_faces * 3;
      aux   = (Msh3DId *)Msh3DMalloc( num_node * 3 * sizeof(Msh3DId) ) ;
      nodes = (double *)Msh3DMalloc( num_node * 3 * sizeof(double) ) ;
      faces = (int *)Msh3DMalloc( num_face * 3 * sizeof(int) ) ;
      num_face = 0;
      num_node = 0;
      for( i = 0; i < num_org_faces; i++ ) {
       for (j = 0; j < 3; j++) {
        Msh3DPushNode (aux,nodes,faces,&num_node,num_face,original_faces[i][j],j);
       }
       num_face++;
      }
      vec_nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       vec_nodes[i] = (double *)Msh3DCalloc( num_node, sizeof(double) ) ;
      for( i = 0; i < num_node; i++ )
      {
       vec_nodes[i][0] = nodes[i*3+0] ;
       vec_nodes[i][1] = nodes[i*3+1] ;
       vec_nodes[i][2] = nodes[i*3+2] ;
      }
      vec_faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for( i = 0; i < num_face; i++ )
       vec_faces[i] = (int *)Msh3DCalloc( num_face, sizeof(int) ) ;
      for( i = 0; i < num_face; i++ )
      {
       vec_faces[i][0] = faces[i*3+0] ;
       vec_faces[i][1] = faces[i*3+1] ;
       vec_faces[i][2] = faces[i*3+2] ;
      }
      Msh3DInternalNodes (num_node,num_face,vec_nodes,vec_faces,&num_gen_nodes,
                        &generated_nodes, 0);
      Msh3DFree( aux ) ;
      Msh3DFree( nodes ) ;
      Msh3DFree( faces ) ;
      for( i = 0; i < num_node; i++ )
       Msh3DFree( vec_nodes[i] ) ;
      Msh3DFree( vec_nodes ) ;
      for( i = 0; i < num_face; i++ )
       Msh3DFree( vec_faces[i] ) ;
      Msh3DFree( vec_faces ) ;

      /* initialize gdb package for shape polyhedron procedure */

#if DSP_MESH
      if ( FDEBUG && gdb ) {
        GdbIni ("Mesh3d Generator - Shape Polyhedron");
        GdbDrawBdry();
        gdb = 0;
      }
      else {
        GdbFit ( ) ;
        GdbClear();
        GdbDrawBdry();
      }
      fprintf (stderr,"Shape Polyhedron\n");
#endif

      /* get the face on the top of the stack. If the stack is empty then
         we are done. */

      while ( face = Msh3DBdryPop() ) {

#if DSP_MESH
       if ( FDEBUG && GdbWait ) {
         GdbReset();
         GdbSetCurFaceOn(face);
         GdbClear();
       }
#endif

       /* examine all active nodes. Rank all nodes as to the goodness
          of the tetrahedral they will form with the current face */

        Msh3DHeapInit( (*nnodes) ) ;
        for ( i=0 ; i<(*nnodes) ; i++ )
        {
            if ( node_list[i].active_flag )
            {
              double d, dist, dist_vec[3], dnew[3], new[3] ;

              /* find the vector from the center to the new point
                 and make sure that this dotted with the normal is
                 positive */

              cand_vec[0] = node_list[i].coord[0] - face->center[0] ;
              cand_vec[1] = node_list[i].coord[1] - face->center[1] ;
              cand_vec[2] = node_list[i].coord[2] - face->center[2] ;

              dot = cand_vec[0]*face->r_x_s[0] +
                    cand_vec[1]*face->r_x_s[1] +
                    cand_vec[2]*face->r_x_s[2] ;

              if ( dot <= 0.0 ) continue ;

              /* verify if node is out of sphere centered in optimal node
                 for the face and ratio equal of largest (smallest)
                 distance from this node to any vertex of the face.
                 This should be doen only the fase of ideal elements. */

              if (face->use == 0)
              {
               Msh3DFindNode (face, 1, new) ;

               dist = 1000000 ;
               for ( j=0; j<3; j++ ) {
                dnew[0] = new[0] - node_list[face->verts[j]].coord[0] ;
                dnew[1] = new[1] - node_list[face->verts[j]].coord[1] ;
                dnew[2] = new[2] - node_list[face->verts[j]].coord[2] ;
                d = sqrt ( dnew[0] * dnew[0] +
                           dnew[1] * dnew[1] +
                           dnew[2] * dnew[2] ) ;
                if ( d < dist ) dist = d ;
               }
               dist /= 1.0 ;

               dist_vec[0] = node_list[i].coord[0] - new[0] ;
               dist_vec[1] = node_list[i].coord[1] - new[1] ;
               dist_vec[2] = node_list[i].coord[2] - new[2] ;
               d = sqrt ( dist_vec[0] * dist_vec[0] +
                          dist_vec[1] * dist_vec[1] +
                          dist_vec[2] * dist_vec[2] ) ;

               if ( d >= dist ) continue ;
              }

              /* the metric we are currently using is the square of
                 the distance from the center of the face to the
                 candidate node */

              if (face->use == 0)
              {
               metric = Msh3DHeapSolidAngle( face, node_list,
                 face->verts[0], face->verts[1], face->verts[2], i ) ;
               if( metric != 0.0 )    /* multiply by -1 because of heap */
                Msh3DHeapInsert( (-1.0*metric), i ) ;
              }
              else
              {
               metric = 0.0 ;
               for ( j=0 ; j<3 ; j++ )
                 metric += cand_vec[j] * cand_vec[j] ;

               metric = metric/dot ;
               Msh3DHeapInsert( metric, i ) ;
              }
            }
        }

        /* start with the node with the best ranking.  Check to make
           sure it will form a valid element (i.e., does not intersect
           the current boundary).  If the element is invalid go on
           to the next.  If the element is valid, then look at the next
           candidate node.  If the two nodes have the same coordinates
           then use topology to determine the one we want. */

          DebugFail = 0 ;
          trials = 0 ;
          valid = 0 ;
	  alloc = 1 ;
          while ( !valid ) {

            /* extract a node from the heap based in its metric */

            node_indx = Msh3DHeapExtract( &metric ) ;
            if ((node_indx == face->verts[0]) ||
                (node_indx == face->verts[1]) ||
                (node_indx == face->verts[2])) continue ;

            /* here a node was found to make the element (node_indx was
               extracted from the heap). Check its validity */

            if ( node_indx != -1 ) {

             /* node avaiable in the heap */

             check = 1 ;

             /* check validity for node choosen */

             if (check)
              check = Msh3DCheckVolum( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckShape( face, node_list, node_indx ) ;
             if (check)
              check = Msh3DCheckValid( face, node_list, node_indx, num_bdry,
                        -1, 0, *elems, nelems ) ;

             /* set validity for node choosen */

             valid = check ;
             if ( valid ) {
                 next_indx = Msh3DHeapExtract( &next_metric ) ;
                 if ( next_indx == -1 ) break ;
                 if (( next_metric == metric ) &&
                     ( node_list[node_indx].coord[0] ==
                       node_list[next_indx].coord[0] ) &&
                     ( node_list[node_indx].coord[1] ==
                       node_list[next_indx].coord[1] ) &&
                     ( node_list[node_indx].coord[2] ==
                       node_list[next_indx].coord[2] )) {
                    /* fprintf( stderr, "Determine The node we want \n" ) ; */
                    node_indx = Msh3DDetNode( face, node_list,
                                              node_indx, next_indx ) ;
                 }
             }
             else {
                trials += 1 ;
             }
            }

            /* here no node was found to make the element (no node_indx was
	       found). Try insertion of new node by an baricentric procedure
               that represents the optimal node for this face */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = 1 ;
             alloc     = 1 ;

             /* try insertion of optimal node if it's in the phase of
                ideal elements */

             if (face->use == 0)
             {
               /* try insertion of new node */

               for (k=1; k<=1; k++)
	       {
                /* node inserted */

                check = Msh3DInsertNewNodesBar (face, nnodes, k,
                          &alloc, &face_pop) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckOptim(face, (*nnodes)-1, num_org_nodes,
                           num_org_faces, original_nodes, original_faces) ;
                if (check)
                 check = Msh3DCheckValid( face, node_list, (*nnodes)-1,
                           num_bdry, -1, 0, *elems, nelems ) ;

                 /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = (*nnodes) - 1 ;
               }
               else {
                (*nnodes)-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }

            /* here no node was found to make the element (no node_indx was
	       found). Try insertion of new node by an adjacent procedure if
	       this face is the first face that went one time to the end of
	       the stack (bdry_try face represented by use equal to 1) */

            if ( node_indx == -1 ) {

             /* no more nodes avaiable in the heap. Try insertion of new ones */

             next_indx = -1 ;
             alloc     =  1 ;

             /* try insertion of optimal node if it's in the phase of
                insert elements */

             if (face->use == 1)
             {
               /* try insertion of new node */

               for (k=1; k<=NEW_NODES; k++)
	       {
                /* node inserted */

                check = Msh3DInsertNewNodesAdj (face, nnodes, k,
                          &alloc, &face_pop, dsp) ;

                /* check validity for node inserted */

                if (check)
                 check = Msh3DCheckValid( face, node_list, (*nnodes)-1,
                           num_bdry, k, 1, *elems, nelems ) ;

                /* it's not necessary try anymore if node is valid */

                if (check) break ;
               }

               /* set validity for node inserted */

               valid = check ;
               if ( valid ) {
                node_indx = (*nnodes) - 1 ;
               }
               else {
                (*nnodes)-- ;
                nintnode-- ;
               }
             }
             if ( next_indx == -1 ) break ;
            }

          }

	  /* if no node was found even with the insertion of new nodes by
	     procedures described above, return to main driver and write
             error code if this face is the first face that went two times
             to to the end of stack (bdry_try face represented by use equal
             to 2) or push this face into the end of stack and try next
             face. Else if a node was found, reset the first face that went
             to the end of stack (bdry_try face) and its use, considering
             that if this face has use equal to 0 then reset bdry_try face
             to no one face and continue trying as before, else if its use
             is equal to 1 then reset bdry_try face to current one and
             continue trying now inserting a new node always when needed */

	  if ( node_indx == -1 ) {
           if (face->use == 2) {
             Msh3DBdryPush (face) ;
             if ( Msh3DCorrePoly (nnodes, nelems,
                                  elems, 0, 1, 0, 1, pjmesh_use_box_test) ) break ;
             Msh3D_errno = Msh3D_INSERT_NO_VALID_NODES ;
             strcpy( Msh3D_err_msg,
              "Msh3DBdryContraction : Insertion of new nodes wasn't enough" ) ;
             return(0);
           }
           else {
             face->use++ ;
             face_pop++ ;
             Msh3DBdryPush (face) ;
             continue ;
           }
          }
	  else {
           if (face->use == 0) {
            face_draw++ ;
            face_done++ ;
            if ( face_done > face_number ) {
            }
           }
          }

       /* add the new element to the element list, update the
          boundary face list, and update the active node list. */

          Msh3DAddElem( nelems, elems, face, node_indx ) ;
          Msh3DAddFaces( face, node_list, node_indx, nelems ) ;
    }
   }

   /* print timing if number of elements changed */

#if DSP_TIME
    if (cur_nelems != *nelems)
    {
     tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
     fprintf( stderr,"\t\t Generated elements: %8d ele.\n", *nelems);
     fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.\n", (double)tot_time);
     fprintf( stderr,"\n") ;
    }
#endif

  }

  /* if we get here all polyhedrons were corrected and the mesh
     was generated */

  return 1 ;
}

/* -------------------------------------------------------------------
** Msh3DNodeList - these routines manage a list that is used to
**                 store the active node data.
**/

static void Msh3DNodeFreeAll(void)
{
    Msh3DFree (node_list) ;
    node_list = NULL ;
}

/* -------------------------------------------------------------------
** Msh3DRangeTree - these routines manage a tree that is used to
**                  make search on faces.
*/

static void Msh3DRangeFreeAll(void)
{
#if defined(AD_TREE) || defined(AD_DEBU)
    Msh3DFree (adtreehash) ;
    CleanRangeTree() ;
    adtreehash = 0 ;
#endif
}

/* -------------------------------------------------------------------
** Msh3DPointTree - these routines manage a tree that is used to
**                  make search on nodes.
*/

static void Msh3DPointFreeAll(void)
{
#if defined(PT_TREE) || defined(PT_DEBU)
    CleanPointTree() ;
#endif
}

/* -------------------------------------------------------------------
** Msh3DPolyFaceStack - these routines manage a stack that is used to
**                      store the active polyhedron face data.
*/

static Msh3DPolyFace Msh3DPolyFaceAlloc(void)
{
    Msh3DPolyFace  new_block, alloced ;
    int            i ;

    /* if the free pointer is null we need to allocate a new block
       of polyhedron faces */

    if ( !poly_free ) {
        new_block = (Msh3DPolyFace)Msh3DMalloc(
           MSH3D_POLY_FACE_QUANTUM * sizeof(Msh3DPolyFaceRec) ) ;
        new_block[0].next = poly_block_ptr ;
        poly_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_POLY_FACE_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_POLY_FACE_QUANTUM-1].next = 0 ;
        poly_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = poly_free ;
    poly_free = poly_free->next ;

    return( alloced ) ;
}

static void Msh3DPolyFree( Msh3DPolyFace face )
{
    /* put this face back on the free list */

    face->next = poly_free ;
    poly_free = face ;
}

static void Msh3DPolyFaceFreeAll(void)
{
    Msh3DPolyFace  cur, next ;

    /* free all blocks allocated to store face information */

    if ( poly_block_ptr ) cur = poly_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    poly_stack = 0 ;
    poly_tail  = 0 ;
    poly_free  = 0 ;
    poly_cursor    = 0 ;
    poly_block_ptr = 0 ;
#if VER_BOTH
    bdry_last  = 0 ;
    bdry_use1  = 0 ;
    bdry_use2  = 0 ;
    bdry_useNeg  = 0 ;
#endif

}

static void Msh3DPolyPush( Msh3DPolyFace poly_face )
{
    /* push this face on the end of the stack that is implemented
       as a doubly linked list */

    poly_face->prev = poly_tail ;
    poly_face->next = 0 ;
    if ( poly_tail ) poly_tail->next = poly_face ;
    poly_tail = poly_face ;
    if ( !poly_stack ) poly_stack = poly_face ;
}

static Msh3DPolyFace Msh3DPolyPop(void)
{
    Msh3DPolyFace  popped ;

    /* pop a face from the front of the stack that is implemented
       as a doubly linked list */

    if (!poly_stack) return(0) ;
    popped = poly_stack ;

    return( popped ) ;
}

static void Msh3DPolyDelete( Msh3DBdryFace face )
{
    Msh3DPolyFace  poly_face = 0 ;

    /* find the given face in polyhedron stack */

    Msh3DPolyReset () ;
    while (poly_face = Msh3DPolyNext ()) {
     if (poly_face->face == face) {
      break ;
     }
    }
    if (poly_face == 0) return ;

    /* delete this face from the middle of the doubly linked list */

    if ( poly_stack == poly_face ) poly_stack = poly_face->next ;
    if ( poly_tail  == poly_face ) poly_tail  = poly_face->prev ;
    if ( poly_face->next ) poly_face->next->prev = poly_face->prev ;
    if ( poly_face->prev ) poly_face->prev->next = poly_face->next ;
    Msh3DPolyFree( poly_face ) ;
}

#if 0 /* Markos */
void Msh3DPolyReset(void)
{
    /* reset the cursor for scanning through the list */

    poly_cursor = poly_stack ;
}

Msh3DPolyFace Msh3DPolyNext(void)
{
    Msh3DPolyFace current ;

    /* return the face the cursor is pointing to, and increment the
       cursor */

    current = poly_cursor ;
    if ( poly_cursor ) poly_cursor = poly_cursor->next ;
    return( current ) ;
}
#endif //#if 0 /* Markos */

/* -------------------------------------------------------------------
** Msh3DFaceStack - these routines manage a stack that is used to store
**                  the active boundary face data.
*/

static Msh3DBdryFace Msh3DFaceAlloc(void)
{
    Msh3DBdryFace  new_block, alloced ;
    int            i ;

    /* if the free pointer is null we need to allocate a new block
       of boundary nodes */

    if ( !bdry_free ) {
        new_block = (Msh3DBdryFace)Msh3DMalloc(
           MSH3D_BDRY_FACE_QUANTUM * sizeof(Msh3DBdryFaceRec) ) ;
        new_block[0].next = bdry_block_ptr ;
        bdry_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_BDRY_FACE_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_BDRY_FACE_QUANTUM-1].next = 0 ;
        bdry_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = bdry_free ;
    bdry_free = bdry_free->next ;

    return( alloced ) ;
}

static void Msh3DBdryFree( Msh3DBdryFace face )
{
    /* put this face back on the free list */

    face->next = bdry_free ;
    bdry_free = face ;
}

//static void PMsh3DFaceFreeAll(void)
void PMsh3DFaceFreeAll(void)
{
    Msh3DBdryFace  cur, next ;

    /* free all blocks allocated to store face information */

    if ( bdry_block_ptr ) cur = bdry_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    bdry_stack = 0 ;
    bdry_tail  = 0 ;
    bdry_free  = 0 ;
    bdry_try   = 0 ;
    bdry_cursor    = 0 ;
    bdry_block_ptr = 0 ;
#if VER_BOTH
    bdry_last  = 0 ;
    bdry_use1  = 0 ;
    bdry_use2  = 0 ;
    bdry_useNeg  = 0 ;
#endif
}

#if 0
static void Msh3DBdryPrint(char *txt)
{
    Msh3DBdryFace  cur ;

    fprintf(stderr,"\n%s\n", txt) ;
    if (!bdry_stack) return ;
    cur = bdry_stack ;
    while ( cur ) {
        fprintf( stderr, "%d %d %d\n", cur->verts[0],
                 cur->verts[1], cur->verts[2] ) ;
        cur = cur->next ;
    }
}
#endif

static void Msh3DBdryDump(Msh3DBdryNodeList nl)
{
   int i;
   Msh3DBdryFace  cur ;

   if (!bdry_stack) return ;

#if DSP_MESH
   if ( FDEBUG) {
     GdbClear ();
     GdbSetWait(0);
     GdbDrawBdry ();
   }
#endif

   cur = bdry_stack ;
   while ( cur ) {

     fprintf( stderr, "define line = {{ %f %f %f } { %f %f %f }}\n",
              nl[cur->verts[0]].coord[0],nl[cur->verts[0]].coord[1],
              nl[cur->verts[0]].coord[2],
              nl[cur->verts[1]].coord[0],nl[cur->verts[1]].coord[1],
              nl[cur->verts[1]].coord[2] ) ;
     fprintf( stderr, "define line = {{ %f %f %f } { %f %f %f }}\n",
              nl[cur->verts[1]].coord[0],nl[cur->verts[1]].coord[1],
              nl[cur->verts[1]].coord[2],
              nl[cur->verts[2]].coord[0],nl[cur->verts[2]].coord[1],
              nl[cur->verts[2]].coord[2] ) ;
     fprintf( stderr, "define line = {{ %f %f %f } { %f %f %f }}\n",
              nl[cur->verts[2]].coord[0],nl[cur->verts[2]].coord[1],
              nl[cur->verts[2]].coord[2],
              nl[cur->verts[0]].coord[0],nl[cur->verts[0]].coord[1],
              nl[cur->verts[0]].coord[2] ) ;
     cur = cur->next ;
  }
}

static void Msh3DBdryPush( Msh3DBdryFace face )
{
    //PMsh3DPrintBdryList("before Msh3DBdryPush\n", face);

    int print = 0 ;

    /* push this face on the end of the stack that is implemented
       as a doubly linked list */

#if DIA_PUSH
    if (face->layer == 0) {
     printf ("Face on the original input pushed back:\n") ;
     printf ("Face use = %d\n", face->use) ;
     printf ("Face (internally) = %d %d %d\n", face->verts[0],
		                               face->verts[1],
		   	                       face->verts[2]) ;
     printf ("Face (actually)   = %d %d %d\n",
		              node_list[face->verts[0]].real_id,
			      node_list[face->verts[1]].real_id,
			      node_list[face->verts[2]].real_id) ;
    }
    else {
     if (print) {
      printf ("Face pushed back:\n") ;
      printf ("Face use = %d\n", face->use) ;
      printf ("Face (internally) = %d %d %d\n", face->verts[0],
		                                face->verts[1],
		   	                        face->verts[2]) ;
      printf ("Face (actually)   = %d %d %d\n",
		              node_list[face->verts[0]].real_id,
			      node_list[face->verts[1]].real_id,
			      node_list[face->verts[2]].real_id) ;
     }
    }
#endif

    face->prev = bdry_tail ;
    face->next = 0 ;
    if ( bdry_tail ) bdry_tail->next = face ;
    bdry_tail = face ;
    if ( !bdry_stack ) bdry_stack = face ;

    //PMsh3DPrintBdryList("after Msh3DBdryPush\n", 0);
}

#if 0
static void Msh3DBdryPushFront( Msh3DBdryFace face )
{
    /* push this face on the front of the stack that is implemented
       as a doubly linked list */

    face->prev = 0 ;
    face->next = bdry_stack ;
    if ( bdry_stack ) bdry_stack->prev = face ;
    bdry_stack = face ;
    if ( !bdry_tail ) bdry_tail = face ;
}
#endif

static void Msh3DBdryPushCorre( Msh3DBdryFace face )
{
    //PMsh3DPrintBdryList("before Msh3DBdryPushCorre\n", face);

    Msh3DBdryFace  bdry_cur = 0 ;
    Msh3DBdryFace  bdry_tes = 0 ;

    /* find the bdry_try, that is, the first face that went one time
       to the end of the stack */

    bdry_try = 0 ;
#if OLD_BTRY
    bdry_cur = bdry_stack;
    do {
     if (bdry_cur == 0) {
      bdry_try = 0 ;
      break ;
     }
     else {
      if (bdry_cur->use > face->use)
      {
       bdry_try = bdry_cur ;
       break ;
      }
      bdry_cur = bdry_cur->next ;
     }
    } while ( bdry_cur != bdry_tail ) ;
#else
    if ((face->use==0) && (bdry_use1!=0)) {
     if (bdry_use1 != bdry_tail)  bdry_try = bdry_use1 ;
     else                         bdry_try = 0 ;
    }
    if ((face->use==1) && (bdry_use2!=0)) {
     if (bdry_use2 != bdry_tail)  bdry_try = bdry_use2 ;
     else                         bdry_try = 0 ;
    }
    if ((face->use==2) && (bdry_useNeg!=0)) {
     if (bdry_useNeg != bdry_tail)  bdry_try = bdry_useNeg ;
     else                           bdry_try = 0 ;
    }
#if DIA_BTRY
    bdry_cur = bdry_stack;
    do {
     if (bdry_cur == 0) {
      bdry_tes = 0 ;
      break ;
     }
     else {
      if (bdry_cur->use > face->use)
      {
       bdry_tes = bdry_cur ;
       break ;
      }
      bdry_cur = bdry_cur->next ;
     }
    } while ( bdry_cur != bdry_tail ) ;
    if (bdry_tes != bdry_try) {
     fprintf (stderr, "Face has a different btry:\n") ;
     fprintf (stderr, "Face (internally) = %d %d %d\n",
		                           face->verts[0],
					   face->verts[1],
					   face->verts[2]) ;
     fprintf (stderr, "Face (actually)   = %d %d %d\n",
		         node_list[face->verts[0]].real_id,
			 node_list[face->verts[1]].real_id,
			 node_list[face->verts[2]].real_id) ;
    }
#endif
#endif

    /* modify the list to push the face in its position considering the
       list only from bdry_stack to bdry_try, because faces from bdry_try
       until bdry_tail are faces already tested and pushed to the end of
       the list */

    if (bdry_try == 0) {
     face->prev = bdry_tail ;
     face->next = 0 ;
     if ( bdry_tail ) bdry_tail->next = face ;
     bdry_tail = face ;
     if ( !bdry_stack ) bdry_stack = face ;
    }
    else {
     face->prev = bdry_try->prev ;
     face->next = bdry_try ;
     if ( bdry_try->prev ) bdry_try->prev->next = face ;
     bdry_try->prev = face ;
     if ( bdry_try == bdry_stack ) bdry_stack = face ;
    }
	//PMsh3DPrintBdryList("after Msh3DBdryPushCorre\n", 0);
	//PMsh3DCheckBdryUse();
}


static void Msh3DBdryPushSmall( Msh3DBdryFace  face )
{
    //PMsh3DPrintBdryList("before Msh3DBdryPushSmall\n", face);

    Msh3DBdryFace  bdry_cur = 0 ;
    Msh3DBdryFace  bdry_real_tail  = 0 ;
    Msh3DBdryFace  bdry_real_stack = 0 ;
    int            found_pos = 0 ;
    double         len, des_area, act_area, l, h, fac, node[3], level ;
    double         tol ;
    Msh3DBdryFace  bdry_tes = 0 ;

    /* define the tolerance for insertion */

#if OLD_PUSH
    tol = 1e-06 ;
#else
    tol = 1e-12 ;
#endif

    /* find the key value for the face */

    face->key = face->area ;

    /* find the bdry_try, that is, the first face that went one time
       to the end of the stack */

#if OLD_BTRY
    bdry_try = 0 ;
    bdry_cur = bdry_stack;
    do {
     if( bdry_cur ) {
      if (bdry_cur->use > 0)
      {
       bdry_try = bdry_cur ;
       break ;
      }
      bdry_cur = bdry_cur->next ;
     }
    } while ( bdry_cur != 0 ) ;
#else
    if (face->layer == 0) {
     bdry_try = 0 ;
    }
    else {
     if      (bdry_use1!=0) bdry_try = bdry_use1 ;
     else if (bdry_use2!=0) bdry_try = bdry_use2 ;
     else if (bdry_useNeg!=0) bdry_try = bdry_useNeg ;
     else                   bdry_try = 0 ;
    }
#endif

    /* modify the list to push the face in its position considering the
       list only from bdry_stack to bdry_try, because faces from bdry_try
       until bdry_tail are faces already tested and pushed to the end of
       the list */

    if ( (bdry_try != 0) && (bdry_try->prev != 0) ) {
     if (bdry_real_tail == 0) {
      bdry_real_tail = bdry_tail ;
      bdry_tail = bdry_try->prev ;
      bdry_tail->next = 0 ;
     }
    }

    /* modify the list to consider bdry_last */

    if ( bdry_last != 0 )
    {
     if (bdry_real_stack == 0) {
      bdry_real_stack = bdry_stack ;
      bdry_stack = bdry_last->next ;
     }
     if (bdry_last == bdry_tail) {
      bdry_real_tail = bdry_tail ;
      bdry_tail = 0 ;
     }
    }

    /* push this face in its right position in the list acoording
       wiht its area. Here is only considered the list from the
       bdry_stack until bdry_try */

    if ( !found_pos ) {
     if ( (bdry_stack != 0) )
     {
      if ( (face->key < bdry_stack->key) ||
	   (ABOUT_ZERO(fabs(face->key-bdry_stack->key), tol)) ) {
       face->prev = 0 ;
       face->next = bdry_stack ;
       if ( bdry_stack ) bdry_stack->prev = face ;
       bdry_stack = face ;
       if ( !bdry_tail ) bdry_tail = face ;
       found_pos = 1 ;
      }
     }
     else
     {
      face->prev = 0 ;
      face->next = bdry_stack ;
      if ( bdry_stack ) bdry_stack->prev = face ;
      bdry_stack = face ;
      if ( !bdry_tail ) bdry_tail = face ;
      found_pos = 1 ;
     }
    }

    if ( !found_pos ) {
     if ( (bdry_tail != 0) )
     {
      if ( (face->key > bdry_tail->key) ||
	   (ABOUT_ZERO(fabs(face->key-bdry_tail->key), tol)) ) {
       face->prev = bdry_tail ;
       face->next = 0 ;
       if ( bdry_tail ) bdry_tail->next = face ;
       bdry_tail = face ;
       found_pos = 1 ;
      }
     }
    }

#if OLD_SMAL
    if ( !found_pos ) {
     bdry_cur = bdry_stack;
     do {
      if ( ABOUT_ZERO(fabs(face->key-bdry_cur->key),tol) ) {
       face->prev = bdry_cur->prev ;
       face->next = bdry_cur ;
       if ( bdry_cur->prev ) bdry_cur->prev->next = face ;
       bdry_cur->prev = face ;
       found_pos = 1 ;
       break ;
      }
      else if ( (face->key > bdry_cur->key) &&
                (face->key < bdry_cur->next->key) ) {
       face->prev = bdry_cur ;
       face->next = bdry_cur->next ;
       if ( bdry_cur->next ) bdry_cur->next->prev = face ;
       bdry_cur->next = face ;
       found_pos = 1 ;
       break ;
      }
      bdry_cur = bdry_cur->next ;
     } while ( bdry_cur != bdry_tail ) ;
    }
#else
    if ( !found_pos ) {
     bdry_cur = bdry_stack;
     do {
      if (bdry_cur == 0) break ;
      if ( ABOUT_ZERO(fabs(face->key-bdry_cur->key),tol) ) {
       face->prev = bdry_cur->prev ;
       face->next = bdry_cur ;
       if ( bdry_cur->prev ) bdry_cur->prev->next = face ;
       bdry_cur->prev = face ;
       found_pos = 1 ;
       break ;
      }
      else if (bdry_cur->next) {
       if ( (face->key > bdry_cur->key) &&
            (face->key < bdry_cur->next->key) ) {
        face->prev = bdry_cur ;
        face->next = bdry_cur->next ;
        if ( bdry_cur->next ) bdry_cur->next->prev = face ;
        bdry_cur->next = face ;
        found_pos = 1 ;
        break ;
       }
      }
      bdry_cur = bdry_cur->next ;
     } while ( bdry_cur != bdry_tail ) ;
    }

    if (!found_pos) {
     fprintf (stderr, "New small insertion in Msh3DBdryPushSmall\n") ;
     face->prev = bdry_tail ;
     face->next = 0 ;
     if ( bdry_tail ) bdry_tail->next = face ;
     bdry_tail = face ;
     if ( !bdry_stack ) bdry_stack = face ;
     found_pos = 1 ;
    }
#endif

    /* test - delete after */
    if( !found_pos )
     fprintf( stderr,"Error in Msh3DBdryPushSmall =\t%d\t%d\t%d\n",
	      face->verts[0], face->verts[1], face->verts[2]) ;

    /* correct the list to push back faces from bdry_try until bdry_tail */

    if ( (bdry_try != 0) && (bdry_try->prev != 0) ) {
     if (bdry_real_tail != 0) {
      bdry_tail->next = bdry_try ;
      bdry_try->prev = bdry_tail ;
      bdry_tail = bdry_real_tail ;
     }
    }

    if (bdry_last != 0) {
     if (bdry_real_stack != 0) {
      bdry_stack->prev = bdry_last ;
      bdry_last->next  = bdry_stack ;
      bdry_stack = bdry_real_stack ;
     }
    }

#if OLD_BTRY
    bdry_try = 0 ;
#else
    if (face->layer != 0) {
     if ((face->use==1) && (face->next==bdry_use1)) bdry_use1 = face ;
     if ((face->use==2) && (face->next==bdry_use2)) bdry_use2 = face ;
     if ((face->use==-1) && (face->next==bdry_useNeg)) bdry_useNeg = face ;
    }
#endif
	//PMsh3DPrintBdryList("after Msh3DBdryPushSmall\n", 0);
	//PMsh3DCheckBdryUse();
}

#if 0
static void Msh3DBdryPushBack( int *face_number )
{
    Msh3DBdryFace  cur ;
    Msh3DBdryFace  end ;
    Msh3DBdryFace  popped ;
    int            number = 0 ;

    /* return if bdry_try is bdry_stack that represents that list is already
       ordered */

    if ( bdry_try == 0 ) return ;
    if ( bdry_try == bdry_stack ) return ;

    /* push faces after bdry_try back to stack in their real position
       according to their area */

    cur = bdry_tail ;
    end = bdry_try->prev ;
    while( cur != end )
    {
     if( cur->prev ) cur->prev->next = cur->next ;
     if( cur->next ) cur->next->prev = cur->prev ;
     popped = cur ;
     cur = cur->prev ;
     bdry_tail = cur ;
     if( bdry_try == popped ) bdry_try = 0 ;
     popped->next = popped->prev = 0 ;
     Msh3DBdryPushSmall ( popped ) ;
     number++ ;
    }

    /* update face number */

    *face_number = number ;
}
#endif

static Msh3DBdryFace Msh3DBdryPop(void)
{
    //PMsh3DPrintBdryList("before Msh3DBdryPop\n", 0);

    Msh3DBdryFace  popped ;
    Msh3DBdryFace  curr ;
    Msh3DBdryFace  bdry_cur, bdry_tes ;
    int            search ;

    /* pop a face from the front of the stack that is implemented
       as a doubly linked list */

#if OLD_BTRY
    /* Pop a face from the front */
    if (!bdry_stack) return(0) ;
    if ( bdry_stack == bdry_tail ) bdry_tail = 0 ;
    popped = bdry_stack ;
    bdry_stack = bdry_stack->next ;
    if ( bdry_stack ) bdry_stack->prev = 0 ;
    popped->next = popped->prev = 0 ;
#else
    /* Update bdry_use1 and bdry_use2 and bdry_useNeg if it is the case */
    if (!bdry_stack) return(0) ;

    if ((bdry_use1) && (bdry_use1->use == -1))
	{
		bdry_use1 = bdry_stack;

		while ((bdry_use1 != 0) && (bdry_use1->use != 1))
		{
			bdry_use1 = bdry_use1->next;
		}
	}
    if ((bdry_use2) && (bdry_use2->use == -1))
	{
		bdry_use2 = bdry_stack;

		while ((bdry_use2 != 0) && (bdry_use2->use != 2))
		{
			bdry_use2 = bdry_use2->next;
		}
	}

    if ( bdry_stack == bdry_use1 ) {
     search = 0 ;
     bdry_use1 = 0 ;
     curr = bdry_stack->next ;
     while ( curr != 0 ) {
      if ( curr->use == 1 ) {
       bdry_use1 = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_use1 higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }
    else if ( bdry_stack == bdry_use2 ) {
     search = 0 ;
     bdry_use2 = 0 ;
     curr = bdry_stack->next ;
     while ( curr != 0 ) {
      if ( curr->use == 2 ) {
       bdry_use2 = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_use2 higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }
    /*else if ( bdry_stack == bdry_useNeg ) {
     search = 0 ;
     bdry_useNeg = 0 ;
     curr = bdry_stack->next ;
     while ( curr != 0 ) {
      if ( curr->use == -1 ) {
       bdry_useNeg = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_useNeg higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }*/
    else if (bdry_stack->use == -1)
	{
		bdry_useNeg = bdry_stack;

		curr = bdry_stack->next;

		while ((curr != 0) && (curr->use == -1))
		{
			curr = curr->next;
		}

		if (curr == 0)
		{
			//PMsh3DPrintBdryList("after Msh3DBdryPop\n", 0);

			return 0;
		}

		if (curr == bdry_use1)
		{
			bdry_use1 = curr->next;

			while ((bdry_use1 != 0) && (bdry_use1->use != 1))
			{
				bdry_use1 = bdry_use1->next;
			}
		}

		if (curr == bdry_use2)
		{
			bdry_use2 = curr->next;

			while ((bdry_use2 != 0) && (bdry_use2->use != 2))
			{
				bdry_use2 = bdry_use2->next;
			}
		}

		if (curr == bdry_tail) bdry_tail = bdry_tail->prev;
		if (curr->next) curr->next->prev = curr->prev;
		if (curr->prev) curr->prev->next = curr->next;

		curr->next = curr->prev = 0;

		//PMsh3DPrintBdryList("after Msh3DBdryPop\n", curr);

		return curr;
	}
    /* Pop a face from the front */
    if (!bdry_stack) return(0) ;
    if ( bdry_stack == bdry_tail ) bdry_tail = 0 ;
    popped = bdry_stack ;
    bdry_stack = bdry_stack->next ;
    if ( bdry_stack ) bdry_stack->prev = 0 ;
    popped->next = popped->prev = 0 ;
#endif

	//PMsh3DPrintBdryList("after Msh3DBdryPop\n", popped);

    return( popped ) ;
}

static Msh3DBdryFace Msh3DBdryPopCorre( Msh3DBdryFace face )
{
    //PMsh3DPrintBdryList("before Msh3DBdryPopCorre\n", 0);

    Msh3DBdryFace  popped ;

    /* pop the given face from the stack that is implemented
       as a doubly linked list */

    if ( bdry_stack == face ) bdry_stack = face->next ;
    if ( bdry_tail  == face ) bdry_tail  = face->prev ;
    if ( face->next ) face->next->prev = face->prev ;
    if ( face->prev ) face->prev->next = face->next ;
    popped = face ;
    popped->next = popped->prev = 0 ;

	//PMsh3DPrintBdryList("after Msh3DBdryPopCorre\n", popped);

    return( popped ) ;
}

static void Msh3DBdryDelete( Msh3DBdryFace face )
{
    /*//debug
    if ((PMsh3DIsInvertedFace(face)) || (PMsh3DIsRemainingFrontFace(face->verts[0], face->verts[1], face->verts[2])))
	{
		printf("face %d %d %d should not be deleted! inverted = %d, remaining = %d\n", face->verts[0], face->verts[1], face->verts[2], PMsh3DIsInvertedFace(face), PMsh3DIsRemainingFrontFace(face->verts[0], face->verts[1], face->verts[2]));
	}
	//endebug*/

    Msh3DBdryFace  curr ;
    int            search ;

    /* Delete this face from the middle of the doubly linked list */

#if OLD_BTRY
    /* Delete the face and update the list */
    if ( bdry_stack == face ) bdry_stack = face->next ;
    if ( bdry_tail  == face ) bdry_tail  = face->prev ;
    if ( face->next ) face->next->prev = face->prev ;
    if ( face->prev ) face->prev->next = face->next ;
    Msh3DBdryFree( face ) ;
#else
    /* Update bdry_use1 and bdry_use2 and bdry_useNeg if it is the case */
    if ( face == bdry_use1 ) {
     search = 0 ;
     bdry_use1 = 0 ;
     curr = face->next ;
     while ( curr != 0 ) {
      if ( curr->use == 1 ) {
       bdry_use1 = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_use1 higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }
    else if ( face == bdry_use2 ) {
     search = 0 ;
     bdry_use2 = 0 ;
     curr = face->next ;
     while ( curr != 0 ) {
      if ( curr->use == 2 ) {
       bdry_use2 = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_use2 higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }
    else if ( face == bdry_useNeg ) {
     search = 0 ;
     bdry_useNeg = 0 ;
     curr = face->next ;
     while ( curr != 0 ) {
      if ( curr->use == -1 ) {
       bdry_useNeg = curr ;
       break ;
      }
#if DIA_BTRY
      search++ ;
      if (search > 1000) {
       printf ("Search for bdry_useNeg higher than 1000\n") ;
      }
#endif
      curr = curr->next ;
     }
    }
    /* Delete the face and update the list */
    if ( bdry_stack == face ) bdry_stack = face->next ;
    if ( bdry_tail  == face ) bdry_tail  = face->prev ;
    if ( face->next ) face->next->prev = face->prev ;
    if ( face->prev ) face->prev->next = face->next ;
    Msh3DBdryFree( face ) ;
#endif
}

#if 0 /* Markos */
static void Msh3DBdryReset(void)
{
    /* reset the cursor for scanning through the list */

    bdry_cursor = bdry_stack ;
}

Msh3DBdryFace Msh3DBdryNext(void)
{
    Msh3DBdryFace current ;

    /* return the face the cursor is pointing to, and increment the
       cursor */

    current = bdry_cursor ;
    if ( bdry_cursor ) bdry_cursor = bdry_cursor->next ;
    return( current ) ;
}
#endif //#if 0 /* Markos */

/* -------------------------------------------------------------------
** Msh3DAdjElemAlloc - these routines manage the allocation and freeing
**                     of adjacent elem list entries
*/

static Msh3DAdjElem Msh3DAdjElemAlloc(void)
{
    Msh3DAdjElem  new_block, alloced ;
    int           i ;

    /* if the free pointer is null we need to allocate a new block
       of elements */

    if ( !adj_elem_free ) {
        new_block = (Msh3DAdjElem)Msh3DMalloc(
           MSH3D_ADJ_ELEM_QUANTUM * sizeof(Msh3DAdjElemRec) ) ;
	if (!new_block) return new_block ;
        new_block[0].next = adj_elem_block_ptr ;
        adj_elem_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_ADJ_ELEM_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_ADJ_ELEM_QUANTUM-1].next = 0 ;
        adj_elem_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = adj_elem_free ;
    adj_elem_free = adj_elem_free->next ;

    return( alloced ) ;
}

#if 0
static void Msh3DAdjElemFree( Msh3DAdjElem elem )
{
    /* put this elem back on the free list */

    elem->next = adj_elem_free ;
    adj_elem_free = elem ;
}
#endif

static void Msh3DAdjElemFreeAll(void)
{
    Msh3DAdjElem  cur, next ;

    /* free all blocks allocated to store elem information */

    if ( adj_elem_block_ptr ) cur = adj_elem_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    adj_elem_free = 0 ;
    adj_elem_block_ptr = 0 ;
}

/* -------------------------------------------------------------------
** Msh3DAdjNodeAlloc - these routines manage the allocation and freeing
**                     of node list entries
*/

static void Msh3DAdjNodeFreeAll(void)
{
    Msh3DBdryNodeList cur ;

    /* free all blocks allocated to store node information */

    cur = node_list ;

    Msh3DFree( cur ) ;

    node_list = NULL ;
}

/* -------------------------------------------------------------------
** Msh3DAdjFaceAlloc - these routines manage the allocation and freeing
**                     of adjacent face list entries
*/

static Msh3DAdjFace Msh3DAdjFaceAlloc(void)
{
    Msh3DAdjFace  new_block, alloced ;
    int           i ;

    /* if the free pointer is null we need to allocate a new block
       of boundary nodes */

    if ( !adj_free ) {
        new_block = (Msh3DAdjFace)Msh3DMalloc(
           MSH3D_ADJ_FACE_QUANTUM * sizeof(Msh3DAdjFaceRec) ) ;
        new_block[0].next = adj_block_ptr ;
        adj_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_ADJ_FACE_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_ADJ_FACE_QUANTUM-1].next = 0 ;
        adj_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = adj_free ;
    adj_free = adj_free->next ;

    return( alloced ) ;
}

static void Msh3DAdjFree( Msh3DAdjFace face )
{
    /* put this face back on the free list */

    face->next = adj_free ;
    adj_free = face ;
}

static void Msh3DAdjFreeAll(void)
{
    Msh3DAdjFace  cur, next ;

    /* free all blocks allocated to store face information */

    if ( adj_block_ptr ) cur = adj_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    adj_free = 0 ;
    adj_block_ptr = 0 ;
}

/* -------------------------------------------------------------------
** Msh3DAdjIniFaceAlloc - these routines manage the allocation and
**                        freeing of initial adjacent face list entries
*/

static Msh3DAdjIniFace Msh3DAdjIniFaceAlloc(void)
{
    Msh3DAdjIniFace  new_block, alloced ;
    int              i ;

    /* if the free pointer is null we need to allocate a new block
       of boundary nodes */

    if ( !adj_ini_free ) {
        new_block = (Msh3DAdjIniFace)Msh3DMalloc(
           MSH3D_ADJ_INI_FACE_QUANTUM * sizeof(Msh3DAdjIniFaceRec) ) ;
        new_block[0].next = adj_ini_block_ptr ;
        adj_ini_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_ADJ_INI_FACE_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_ADJ_INI_FACE_QUANTUM-1].next = 0 ;
        adj_ini_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = adj_ini_free ;
    adj_ini_free = adj_ini_free->next ;

    return( alloced ) ;
}

#if 0
static void Msh3DAdjIniFree( Msh3DAdjIniFace face )
{
    /* put this face back on the free list */

    face->next = adj_ini_free ;
    adj_ini_free = face ;
}
#endif

static void Msh3DAdjIniFreeAll(void)
{
    Msh3DAdjIniFace  cur, next ;

    /* free all blocks allocated to store face information */

    if ( adj_ini_block_ptr ) cur = adj_ini_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    adj_ini_free = 0 ;
    adj_ini_block_ptr = 0 ;
}

/* -------------------------------------------------------------------
** Msh3DTestStack - these routines manage a stack that is used to store
**                  the boundary face data already done.
*/

static Msh3DBdryFace Msh3DTestPush(void)
{
    Msh3DBdryFace  face ;

    /* push this face on the end of the list */

    face = Msh3DTestAlloc() ;

    if( test_tail ) test_tail->next = face ;
    face->next = 0 ;
    test_tail  = face ;
    if( !test_cursor ) test_cursor = face ;

    return face ;
}

static Msh3DBdryFace Msh3DTestAlloc(void)
{
    Msh3DBdryFace  new_block, alloced ;
    int            i ;

    /* if the free pointer is null we need to allocate a new block
       of boundary nodes */

    if ( !test_free ) {
        new_block = (Msh3DBdryFace)Msh3DMalloc(
           MSH3D_TEST_FACE_QUANTUM * sizeof(Msh3DBdryFaceRec) ) ;
        new_block[0].next = test_block_ptr ;
        test_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_TEST_FACE_QUANTUM-1) ; i++ ) {
            new_block[i].next = &(new_block[i+1]) ;
        }
        new_block[MSH3D_TEST_FACE_QUANTUM-1].next = 0 ;
        test_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = test_free ;
    test_free = test_free->next ;

    return( alloced ) ;
}

static void Msh3DTestFree( Msh3DBdryFace face )
{
    /* put this face back on the free list */

    face->next = test_free ;
    test_free = face ;
}

static void Msh3DTestFreeAll(void)
{
    Msh3DBdryFace  cur, next ;

    /* free all blocks allocated to store face information */

    if ( test_block_ptr ) cur = test_block_ptr ;
    else return ;

    while ( cur->next ) {
        next = cur->next ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    test_free  = 0 ;
    test_tail  = 0 ;
    test_cursor    = 0 ;
    test_block_ptr = 0 ;
}

/* -------------------------------------------------------------------
** Msh3DAddElem - add a new element to the element list.
*/

static void Msh3DAddElem(
    int           *num_elems,
    int           **elements,
    Msh3DBdryFace face,
    int           node_indx )
{
    int    *slot ;

    /* allocate new space if we need it */

    if ( *num_elems >= num_elem_alloced ) {
        if ( !num_elem_alloced ) {
            num_elem_alloced = MSH3D_ELEM_QUANTUM ;
            *elements = (int *)Msh3DMalloc(
                 num_elem_alloced * 4 * sizeof(int)) ;
        }
        else {
            num_elem_alloced += MSH3D_ELEM_QUANTUM ;
            *elements = (int *)Msh3DRealloc( *elements,
                 num_elem_alloced * 4 * sizeof(int)) ;
        }
    }

    /* store away the vertex numbers */

    slot = &(*elements)[4*(*num_elems)] ;
    slot[0] = face->verts[0] ;
    slot[1] = face->verts[1] ;
    slot[2] = face->verts[2] ;
    slot[3] = node_indx ;
    (*num_elems)++ ;
}

/* -------------------------------------------------------------------
** Msh3DCheckVolum - Check the volume of added element.
*/

static double Msh3DVolumFace(
    Msh3DBdryNodeList  node_list,
    int                base_v,
    int                v[3] )
{
    int                i ;
    double             vol, area[3], cross[3] ;

    /* compute the area for the face */

    area[0] = area[1] = area[2] = 0.0 ;
    for( i = 0; i < 3; i++ ) {

     cross[0] = node_list[v[i]].coord[1] * node_list[v[(i+1)%3]].coord[2] -
                node_list[v[i]].coord[2] * node_list[v[(i+1)%3]].coord[1] ;
     cross[1] = node_list[v[i]].coord[2] * node_list[v[(i+1)%3]].coord[0] -
                node_list[v[i]].coord[0] * node_list[v[(i+1)%3]].coord[2] ;
     cross[2] = node_list[v[i]].coord[0] * node_list[v[(i+1)%3]].coord[1] -
                node_list[v[i]].coord[1] * node_list[v[(i+1)%3]].coord[0] ;

     area[0] += ((1/6.) * cross[0]) ;
     area[1] += ((1/6.) * cross[1]) ;
     area[2] += ((1/6.) * cross[2]) ;
    }

    /* compute the volume contribution for the face */

    vol = area[0] * node_list[base_v].coord[0] +
          area[1] * node_list[base_v].coord[1] +
          area[2] * node_list[base_v].coord[2] ;

    return vol ;
}

static double Msh3DVolum(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int                verts[3] ;
    double             ind_vol = 0.0 ;

    /* compute the volume of tetrahedron */

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[2] ;
    verts[2] = face->verts[1] ;
    ind_vol += Msh3DVolumFace( node_list, verts[0], verts ) ;

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[1] ;
    verts[2] = node_indx ;
    ind_vol += Msh3DVolumFace( node_list, verts[0], verts ) ;

    verts[0] = face->verts[1] ;
    verts[1] = face->verts[2] ;
    verts[2] = node_indx ;
    ind_vol += Msh3DVolumFace( node_list, verts[0], verts ) ;

    verts[0] = face->verts[2] ;
    verts[1] = face->verts[0] ;
    verts[2] = node_indx ;
    ind_vol += Msh3DVolumFace( node_list, verts[0], verts ) ;

    /* return the volume of tetrahedron */

    return ind_vol ;
}

static double Msh3DVolumTetra(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int                i ;
    int                verts[4] ;
    double             p10[3], p21[3], p31[3] ;
    double             fac1[3], fac2[3] ;
    double             ind_vol = 0.0 ;

    /* compute the volume of tetrahedron */

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[1] ;
    verts[2] = face->verts[2] ;
    verts[3] = node_indx ;

    for (i = 0; i < 3; i++) {
     p10[i] = node_list[verts[1]].coord[i] - node_list[verts[0]].coord[i] ;
     p21[i] = node_list[verts[2]].coord[i] - node_list[verts[1]].coord[i] ;
     p31[i] = node_list[verts[3]].coord[i] - node_list[verts[1]].coord[i] ;
    }

    fac1[0] = (1/6.0) * p10[0] ;
    fac1[1] = (1/6.0) * p10[1] ;
    fac1[2] = (1/6.0) * p10[2] ;
    fac2[0] = p21[1] * p31[2] - p31[1] * p21[2] ;
    fac2[1] = p21[2] * p31[0] - p31[2] * p21[0] ;
    fac2[2] = p21[0] * p31[1] - p31[0] * p21[1] ;

    ind_vol = fac1[0] * fac2[0] +
              fac1[1] * fac2[1] +
              fac1[2] * fac2[2] ;

    /* return the volume of tetrahedron */

    return ind_vol ;
}

static int Msh3DCheckVolum(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int                verts[3] ;
    double             node[3], old[3] ;
    double             vol = 0.0 ;
    double             min_vol = 0.0 ;

    /* return if it's not the phase of ideal elements */

    if (face->use != 0) return 1 ;

    /* compute the volume of tetrahedron */

    vol = Msh3DVolumTetra( face, node_list, node_indx ) ;

    /* compute the minimal node */

#if 1
    Msh3DFindNode (face,10,node) ;
#else
    Msh3DFindNode (face,2,node) ;
#endif

    /* save node_indx choosen */

    old[0] = node_list[node_indx].coord[0] ;
    old[1] = node_list[node_indx].coord[1] ;
    old[2] = node_list[node_indx].coord[2] ;

    /* set new node_indx */

    node_list[node_indx].coord[0] = node[0] ;
    node_list[node_indx].coord[1] = node[1] ;
    node_list[node_indx].coord[2] = node[2] ;

    /* compute the minimal volume */

    min_vol = Msh3DVolumTetra( face, node_list, node_indx ) ;

    /* restore node_indx */

    node_list[node_indx].coord[0] = old[0] ;
    node_list[node_indx].coord[1] = old[1] ;
    node_list[node_indx].coord[2] = old[2] ;

    /* Compare the two volumes */

    if( fabs(vol) < fabs(min_vol) )
     return 0 ;
    else
     return 1 ;

}

/* -------------------------------------------------------------------
** Msh3DCheckShape - Check the shape of added element.
*/

static double Msh3DSrmsEdge( Msh3DBdryNodeList node_list, int v[2] )
{
    int                i ;
    double             edg_rms, len, vec[3] ;

    /* get the edge vector */

    for( i = 0; i < 3; i++ ) {
     vec[i] = node_list[v[1]].coord[i] - node_list[v[0]].coord[i] ;
    }

    /* compute the length for the edge */

    len = sqrt( vec[0] * vec[0] +
                vec[1] * vec[1] +
                vec[2] * vec[2] ) ;

    /* compute the rms contribution for the edge */

    edg_rms = ((1/6.0) * len * len) ;

    return edg_rms ;
}

static double Msh3DSrms(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int                verts[2] ;
    double             ind_rms = 0.0 ;

    /* compute the rms of tetrahedron */

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[1] ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[2] ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    verts[0] = face->verts[0] ;
    verts[1] = node_indx ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    verts[0] = face->verts[1] ;
    verts[1] = face->verts[2] ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    verts[0] = face->verts[1] ;
    verts[1] = node_indx ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    verts[0] = face->verts[2] ;
    verts[1] = node_indx ;
    ind_rms += Msh3DSrmsEdge( node_list, verts ) ;

    ind_rms = sqrt( ind_rms ) ;

    /* return the rms of tetrahedron */

    return ind_rms ;
}

static int Msh3DCheckShape(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int                v1i, v2i, v1, v2, i, j, found, test, new = 0 ;
    Msh3DAdjFace       cur ;
    double             ind_vol = 0.0 ;
    double             ind_rms = 0.0 ;
    double             ind_sha = 0.0 ;
#if 1
    if( 1 || face->use == 0 ) return 1 ;
#endif

    /* compute the volume of tetrahedron */

    ind_vol = Msh3DVolum( face, node_list, node_indx ) ;

    /* compute the root_mean_square of tetrahedron */

    ind_rms = Msh3DSrms( face, node_list, node_indx ) ;

    /* compute shape of tetrahedron */

    ind_sha = (ind_rms * ind_rms * ind_rms) / (ind_vol) ;

    /* The node only will be choosen if it's between two limits */

    if( 1 ) {
     if( (ind_sha > (SHA_FACTOR / (MUL_FACTOR*2.0))) &&
         (ind_sha < (SHA_FACTOR * (MUL_FACTOR*2.0))) ) {
      return 1 ;
     }
     else {
      return 0 ;
     }
    }
    else {
     return 1 ;
    }
}

/* -------------------------------------------------------------------
** Msh3DCheckOptim - Check the valid for the optimal point inserted.
*/


int Msh3DCheckOptim(
    Msh3DBdryFace     face,
    int               indx,
    int               num_org_nodes,
    int               num_org_faces,
    double            **original_nodes,
    int               **original_faces )
{
    Msh3DBdryFace  current ;
    int            i, j, k, valid, type ;
    double         angle, d, dist, dist_vec[3], dnew[3], new[3] ;
    double         area, length, height, norm, d_pit, d_cen ;
    double         coords[3][3], cross[3], pit[3], r[3], s[3] ;
    double         cen[3], cmid[3] ;
    double         vec[3], dot ;


    /* test if the node is outside of the body */

#if 0
    angle = 0.0 ;
    for( i = 0; i < num_org_faces; i++ )
    {
     angle += Msh3DHeapSolidAngleTria( original_faces[i][0],
      original_faces[i][1], original_faces[i][2], indx ) ;
    }
    if( !((angle>2*PI)||(angle<-2*PI)) )
     return 0 ;   /* node outside of the body */
#endif

    return 1 ;

     /* get node indx */

     if (indx != (nbdrynode + nintnode - 1) ) return 1 ;

     cen[0] = node_list[indx].coord[0] ;
     cen[1] = node_list[indx].coord[1] ;
     cen[2] = node_list[indx].coord[2] ;

    /* loop through all the faces in the current boundary and see
       if node being created is too near to a existent face */

    valid = 1 ;
    Msh3DBdryReset() ;

    while ( valid && (current = Msh3DBdryNext()) ) {

     /* first verify if this node is inside of the sphere for the
        optimum point. We can reject most faces this way */

     dist_vec[0] = cen[0] - current->center[0] ;
     dist_vec[1] = cen[1] - current->center[1] ;
     dist_vec[2] = cen[2] - current->center[2] ;

     dot = dist_vec[0]*current->r_x_s[0] +
           dist_vec[1]*current->r_x_s[1] +
           dist_vec[2]*current->r_x_s[2] ;

     if ( dot <= 0.0 ) continue ;

     Msh3DFindNode (current, 1, new) ;
     new[0] = current->center[0] ;
     new[1] = current->center[1] ;
     new[2] = current->center[2] ;
     dist = 1000000 ;
     for ( j=0; j<3; j++ ) {
      dnew[0] = new[0] - node_list[current->verts[j]].coord[0] ;
      dnew[1] = new[1] - node_list[current->verts[j]].coord[1] ;
      dnew[2] = new[2] - node_list[current->verts[j]].coord[2] ;
      d = sqrt ( dnew[0] * dnew[0] +
                 dnew[1] * dnew[1] +
                 dnew[2] * dnew[2] ) ;
      if ( d < dist ) dist = d ;
      dist /= 1.0 ;
     }
     dist_vec[0] = cen[0] - current->center[0] ;
     dist_vec[1] = cen[1] - current->center[1] ;
     dist_vec[2] = cen[2] - current->center[2] ;
     d = sqrt ( dist_vec[0] * dist_vec[0] +
                dist_vec[1] * dist_vec[1] +
                dist_vec[2] * dist_vec[2] ) ;

     /* Verify the volume for this face */

     if ( d <= dist )
      type = 0 ;

     if (type == 0) return 0 ;
    }

    /* if we get here, the node is valid */

    return 1 ;
}

/* -------------------------------------------------------------------
** Msh3DCheckVisib - Check the visibility for point inserted in polyhedron.
*/

static int Msh3DCheckVisib( int node_indx, Msh3DBdryFace *visi )
{
    Msh3DPolyFace       cur_poly ;
    Msh3DBdryFace       face ;
    Msh3DBdryFace       unvi ;
    int                 i, j, ve, check, use, bdry, f[3] ;
    int                 found_node, type ;
    int                 face_node[10000], nface_node = 0 ;
    double              node[3] ;
    double              a[3], b[3], c[3], d[3], u[3], v[3], n[3], dot ;

    /* get the coordinates of the given node */

    for (i = 0; i < 3; i++)
     node[i] = node_list[node_indx].coord[i] ;

    /* traverse the polyhedron structure to find all polyhedron nodes
       and set all faces use to zero */

    Msh3DPolyReset () ;
    while (cur_poly = Msh3DPolyNext ()) {
     face = cur_poly->face ;
#if OLD_VISI
     face->use = 0 ;
#else
     if ((*visi == 0) ) face->use = 0 ;
#endif
     for (i = 0; i < 3; i++) {
      ve = face->verts[i] ;
      found_node = 0 ;
      for (j = 0; j < nface_node; j++) {
       if (face_node[j] == ve) {
        found_node = 1 ;
        break ;
       }
      }
      if (!found_node) {
       face_node[nface_node++] = ve ;
      }
     }
    }

    /* test the visibility of the given node. This test is done verifying
       if the line defined by given node and each polyhedron node intersects
       any polyhedron face; if an intersection is found, then the node is
       not visible. */

    check = 1 ;
    use   = 0 ;
    unvi  = 0 ;
    for (i = 0; i < nface_node; i++) {
     for (j = 0; j < 3; j++) {
      u[j] = node[j] ;
      v[j] = node_list[face_node[i]].coord[j] ;
     }
     Msh3DPolyReset () ;
     while (cur_poly = Msh3DPolyNext ()) {
      face = cur_poly->face ;
      for (j = 0; j < 3; j++) {
       a[j] = node_list[face->verts[0]].coord[j] ;
       b[j] = node_list[face->verts[1]].coord[j] ;
       c[j] = node_list[face->verts[2]].coord[j] ;
       f[j] = face->verts[j] ;
      }

      /*fprintf (stderr,"markos debug, visibility test\n");
      fprintf (stderr,"markos debug, edge[0] is (%f, %f, %f)\n", u[0], u[1], u[2]);
      fprintf (stderr,"markos debug, edge[1] is (%f, %f, %f)\n", v[0], v[1], v[2]);
      fprintf (stderr,"markos debug, triangle[0] is (%f, %f, %f)\n", a[0], a[1], a[2]);
      fprintf (stderr,"markos debug, triangle[1] is (%f, %f, %f)\n", b[0], b[1], b[2]);
      fprintf (stderr,"markos debug, triangle[2] is (%f, %f, %f)\n", c[0], c[1], c[2]);*/

      type = GeoTriSegIntersect(1,a,b,c,u,v) ;

      /*//markos debug
      if (type == 1) fprintf (stderr,"no interception\n");
      else fprintf (stderr,"interception\n");
      //end markos debug*/

#if OLD_VISI
      if (type == 0) {
       face->use++ ;
       check = 0 ;
       if ((*visi) != 0) {
        return 0 ;
       }
       else {
        bdry  = Msh3DBoundaryFace (face, f) ;
        if( bdry == 0 ) {
         if( face->use > use ) {
          unvi = face ;
          use  = face->use ;
         }
        }
       }
      }
#else
      if (type == 0) {
       check = 0 ;
       if ((*visi) != 0) {
        return 0 ;
       }
       else {
        face->use++ ;
        bdry  = Msh3DBoundaryFace (face, f) ;
        if( bdry == 0 ) {
         if( face->use > use ) {
          unvi = face ;
          use  = face->use ;
         }
        }
       }
      }
#endif
     }
    }

    /* verify if an unvisible face was found. This is done checking if
       the use of the most unvisible face is higher than zero. */

    if (!check) {
     if ((*visi) == 0) *visi = unvi ;
     return 0 ;
    }
    else {
     return 1 ;
    }
}

/* -------------------------------------------------------------------
** Msh3DCheckPoint - Check the shape for point inserted in polyhedron.
*/

static int Msh3DCheckPoint( int node_indx, int pok )
{
    Msh3DPolyFace       poly_face ;
    Msh3DBdryFace       bdry_face ;
    Msh3DBdryFaceRec    new ;
    double              ind_vol = 0.0 ;
    double              ind_rms = 0.0 ;
    double              ind_sha = 0.0 ;
    int                 check, valid, step = 7 ;

    /* return if point inserted is ok, that is, no check is necessary */

    if( pok ) return 1 ;

    /* verify if the star-shaped polyhedron will create good elements */

    Msh3DPolyReset () ;
    while (poly_face = Msh3DPolyNext ()) {
     bdry_face = poly_face->face ;
     new.verts[0] = bdry_face->verts[0] ;
     new.verts[1] = bdry_face->verts[1] ;
     new.verts[2] = bdry_face->verts[2] ;
     ind_vol = Msh3DVolum( &new, node_list, node_indx ) ;
     ind_rms = Msh3DSrms( &new, node_list, node_indx ) ;
     ind_sha = (ind_rms * ind_rms * ind_rms) / (ind_vol) ;
     if( (ind_sha > (SHA_FACTOR / (MUL_FACTOR+(05*(step-1)))) ) &&
         (ind_sha < (SHA_FACTOR * (MUL_FACTOR+(05*(step-1)))) ) )
     {
      check = 1 ;
      continue ;
     }
     else
     {
      check = 0 ;
      break ;
     }
    }

    /* set validity for node inserted */

    valid = check ;
    if (valid) {
     return 1 ;
    }
    else {
     return 0 ;
    }
}

/* -------------------------------------------------------------------
** Msh3DCheckConsi - this routine makes consistency checks to make sure
**                   that the element is valid.
*/

static int Msh3DConsiFace( Msh3DBdryNodeList node_list, int v[4] )
{
    int    i, j, con ;
    double dot, r[3], s[3], center[3], cross[3] ;

    /* compute the center for the face */

    for( i = 0; i < 3; i++ ) {
     center[i] = 0.0 ;
     for( j = 0; j < 3; j++ ) center[i] += node_list[v[j]].coord[i] ;
     center[i] /= 3.0 ;
    }

    /* compute the normal for the face */

    for( i = 0; i < 3; i++ ) {
     r[i] = node_list[v[1]].coord[i] - node_list[v[0]].coord[i] ;
     s[i] = node_list[v[2]].coord[i] - node_list[v[0]].coord[i] ;
    }
    cross[0] = r[1] * s[2] - s[1] * r[2] ;
    cross[1] = r[2] * s[0] - s[2] * r[0] ;
    cross[2] = r[0] * s[1] - s[0] * r[1] ;

    /* compute the dot between normal and vector from fourth node to
      center for the face */

    for( i = 0; i < 3; i++ ) {
     r[i] = cross[i] ;
     s[i] = node_list[v[3]].coord[i] - center[i] ;
    }
    dot = r[0] * s[0] + r[1] * s[1] + r[2] * s[2] ;
    if( dot <= 0.0 ) con = 0 ;
    else             con = 1 ;

    return con ;
}

static int Msh3DConsi(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int   ind_con ;
    int   verts[4] ;

    /* compute the consistency of tetrahedron */

    verts[0] = face->verts[1] ;
    verts[1] = node_indx ;
    verts[2] = face->verts[2] ;
    verts[3] = face->verts[0] ;
    ind_con  = Msh3DConsiFace( node_list, verts ) ;
    if( !ind_con ) return ind_con ;

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[2] ;
    verts[2] = node_indx ;
    verts[3] = face->verts[1] ;
    ind_con  = Msh3DConsiFace( node_list, verts ) ;
    if( !ind_con ) return ind_con ;

    verts[0] = face->verts[0] ;
    verts[1] = node_indx ;
    verts[2] = face->verts[1] ;
    verts[3] = face->verts[2] ;
    ind_con  = Msh3DConsiFace( node_list, verts ) ;
    if( !ind_con ) return ind_con ;

    verts[0] = face->verts[0] ;
    verts[1] = face->verts[1] ;
    verts[2] = face->verts[2] ;
    verts[3] = node_indx ;
    ind_con  = Msh3DConsiFace( node_list, verts ) ;
    if( !ind_con ) return ind_con ;

    /* return the consistency of tetrahedron */

    return ind_con ;
}

static int  Msh3DCheckConsi(
    Msh3DBdryFace      face,
    Msh3DBdryNodeList  node_list,
    int                node_indx )
{
    int ind_con ;

    /* compute the consistency of tetrahedron */

    ind_con = Msh3DConsi( face, node_list, node_indx ) ;

    /* return the consistency of tetrahedron */

    return ind_con ;
}

static void  Mesh3DDiaDelf (int *elem, int *nelem)
{
  Msh3DBdryFace face ;
  Msh3DPolyFace cur_poly ;
  int  ed[4], vd[3], ind, i, j, found ;
  int  size_node, size_face, *vect_node ;
  FILE *fp1, *fp2 ;

  /* print current boundary information */
  fp1 = fopen ("diabdry.jdia", "w") ;
  fprintf (fp1, "Current boundary faces: \n\n") ;
  size_node = 0 ;
  size_face = 0 ;
  Msh3DBdryReset() ;
  while (face = Msh3DBdryNext() ) {
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   fprintf( fp1,"Face (internally) = %d %d %d\n", vd[0], vd[1], vd[2]) ;
   fprintf( fp1,"Face (actually)   = %d %d %d\n", node_list[vd[0]].real_id,
		                                  node_list[vd[1]].real_id,
					          node_list[vd[2]].real_id) ;
   fprintf (fp1, "\n") ;
   size_node += 3 ;
   size_face += 1 ;
  }
  fclose (fp1) ;
  fp2 = fopen ("diabdry.jmsh", "w") ;
  fprintf( fp2, "%f\n%f\n%d\n%d\n", 0.4, 0.001, 1, 1) ;
  fprintf( fp2, "%d\n", size_node) ;
  vect_node = (int *) Msh3DCalloc (size_node, sizeof (int)) ;
  size_node = 0 ;
  Msh3DBdryReset() ;
  while (face = Msh3DBdryNext() ) {
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   for (i = 0; i < 3; i++) {
    found = 0 ;
    for (j = 0; j < size_node; j++) {
     if (node_list[vd[i]].real_id == vect_node[j]) {
      found = 1 ;
      break ;
     }
    }
    if (!found) {
     fprintf( fp2, "%d %lf %lf %lf\n", node_list[vd[i]].real_id,
		                       node_list[vd[i]].coord[0],
		                       node_list[vd[i]].coord[1],
		                       node_list[vd[i]].coord[2]) ;
     vect_node[size_node] = node_list[vd[i]].real_id ;
     size_node++ ;
    }
   }
  }
  Msh3DFree (vect_node) ;
  fprintf( fp2, "%d\n", size_face) ;
  Msh3DBdryReset() ;
  while (face = Msh3DBdryNext() ) {
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   fprintf( fp2, "%d %d %d\n", node_list[vd[0]].real_id,
		               node_list[vd[1]].real_id,
			       node_list[vd[2]].real_id) ;
  }
  fclose (fp2) ;
  /* print current polyhedron information */
  fp1 = fopen ("diapoly.jdia", "w") ;
  fprintf (fp1, "Current polyhedron faces: \n\n") ;
  size_node = 0 ;
  size_face = 0 ;
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   fprintf( fp1,"Face (internally) = %d %d %d\n", vd[0], vd[1], vd[2]) ;
   fprintf( fp1,"Face (actually)   = %d %d %d\n", node_list[vd[0]].real_id,
	 	                                  node_list[vd[1]].real_id,
					          node_list[vd[2]].real_id) ;
   fprintf (fp1, "\n") ;
   size_node += 3 ;
   size_face += 1 ;
  }
  fclose (fp1) ;
  fp2 = fopen ("diapoly.jmsh", "w") ;
  fprintf( fp2, "%f\n%f\n%d\n%d\n", 0.4, 0.001, 1, 1) ;
  fprintf( fp2, "%d\n", size_node) ;
  vect_node = (int *) Msh3DCalloc (size_node, sizeof (int)) ;
  size_node = 0 ;
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   for (i = 0; i < 3; i++) {
    found = 0 ;
    for (j = 0; j < size_node; j++) {
     if (node_list[vd[i]].real_id == vect_node[j]) {
      found = 1 ;
      break ;
     }
    }
    if (!found) {
     fprintf( fp2, "%d %lf %lf %lf\n", node_list[vd[i]].real_id,
		                       node_list[vd[i]].coord[0],
		                       node_list[vd[i]].coord[1],
		                       node_list[vd[i]].coord[2]) ;
     vect_node[size_node] = node_list[vd[i]].real_id ;
     size_node++ ;
    }
   }
  }
  Msh3DFree (vect_node) ;
  fprintf( fp2, "%d\n", size_face) ;
  Msh3DPolyReset() ;
  while ( cur_poly = Msh3DPolyNext() ) {
   face = cur_poly->face ;
   vd[0] = face->verts[0] ;
   vd[1] = face->verts[1] ;
   vd[2] = face->verts[2] ;
   fprintf( fp2, "%d %d %d\n", node_list[vd[0]].real_id,
		               node_list[vd[1]].real_id,
			       node_list[vd[2]].real_id) ;
  }
  fclose (fp2) ;
  /* print current element information */
  fp1 = fopen ("diaelem.jdia", "w") ;
  fprintf (fp1, "Generated elements: \n\n") ;
  for (i = 0; i < *nelem; i++) {
   ind = i*4 ;
   ed[0] = elem[ind+0] ;
   ed[1] = elem[ind+1] ;
   ed[2] = elem[ind+2] ;
   ed[3] = elem[ind+3] ;
   if ((ed[0]!=-1) && (ed[1]!=-1) && (ed[2]!=-1) && (ed[3]!=-1)) {
    fprintf( fp1,"Elem (internally) = %d %d %d %d\n",ed[0],ed[1],ed[2],ed[3] ) ;
    fprintf( fp1,"Elem (actually)   = %d %d %d %d\n",
		                    node_list[ed[0]].real_id,
		                    node_list[ed[1]].real_id,
		                    node_list[ed[2]].real_id,
		                    node_list[ed[3]].real_id) ;
    fprintf( fp1,"\n") ;
   }
  }
  fclose (fp1) ;
  fp2 = fopen ("diaelem.jmsh", "w") ;
  for (i = 0; i < *nelem; i++) {
   ind = i*4 ;
   ed[0] = elem[ind+0] ;
   ed[1] = elem[ind+1] ;
   ed[2] = elem[ind+2] ;
   ed[3] = elem[ind+3] ;
   if ((ed[0]!=-1) && (ed[1]!=-1) && (ed[2]!=-1) && (ed[3]!=-1)) {
    for (j = 0; j < 4; j++) {
     fprintf( fp2, "%d %lf %lf %lf\n", node_list[ed[j]].real_id,
		                       node_list[ed[j]].coord[0],
		                       node_list[ed[j]].coord[1],
		                       node_list[ed[j]].coord[2]) ;
    }
   }
  }
  for (i = 0; i < *nelem; i++) {
   ind = i*4 ;
   ed[0] = elem[ind+0] ;
   ed[1] = elem[ind+1] ;
   ed[2] = elem[ind+2] ;
   ed[3] = elem[ind+3] ;
   if ((ed[0]!=-1) && (ed[1]!=-1) && (ed[2]!=-1) && (ed[3]!=-1)) {
    for (j = 0; j < 4; j++) {
     if (j == 0) {  /* base face   -> point inwards (to the element) */
      vd[0] = ed[j] ;
      vd[1] = ed[(j+1)%4] ;
      vd[2] = ed[(j+2)%4] ;
     } else {       /* other faces -> point inwards (to the element) */
      vd[0] = ed[j] ;
      vd[1] = ed[(j+2)%4] ;
      vd[2] = ed[(j+1)%4] ;
     }
     fprintf( fp2, "%d %d %d\n", node_list[vd[0]].real_id,
		                 node_list[vd[1]].real_id,
				 node_list[vd[2]].real_id) ;
    }
   }
  }
  fclose (fp2) ;
}

