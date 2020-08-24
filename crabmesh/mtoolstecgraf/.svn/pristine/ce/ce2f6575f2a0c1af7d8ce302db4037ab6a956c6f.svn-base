#ifndef _PJMESH_MAIN_DEF3D_H_
#define _PJMESH_MAIN_DEF3D_H_
//header guard added by Markos

/*
** ----------------------------------------------------------------------
**
** JmeshMainDef3D.h - Header module.
**
** ----------------------------------------------------------------------
**
** Description:
**  This is a header file for the 3D meshing routines.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**   It was based on Wash's and Tilio's initial implementatiom.
**  Modified:	Dec-10-1999	Joaquim Bento Cavalcante Neto
**   Changed function Msh3DGeneration to include a new variable ver
**   to define the jmesh version to be used. The variables internal
**   and factor were also changed by oct and fac, respectively.
**  Modified:	Aug-31-2002	Joaquim Bento Cavalcante Neto
**   Modified struct Msh3DBdryNodeRec to include node id. This was
**   done to allow the printing of the real id nodes including models
**   with jumps in node numbering.
**  Modified:	Feb-19-2003	Joaquim Bento Cavalcante Neto
**   Modified struct Msh3DBdryNodeRec to include mates, which will
**   define mate nodes, if any, of a given node, in case of cracks.
**   The first position of the mate vector (mates[0]) is the number
**   of mates and the remaining positions are the ids of the mates.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Included _cplusplus to get rid of all compiler's warnings.
**
** ----------------------------------------------------------------------
**
*/

#include "Jmesh/JmeshMainDef3D.h"

#ifdef __cplusplus

namespace PJmesh
{
	extern "C" {
#endif

//added by markos
#ifdef VER_BOTH
#undefine VER_BOTH
#endif

#ifdef VER_PRNT
#undefine VER_PRNT
#endif

#define VER_BOTH        1       /* Try Jmesh with both versions */
#define VER_PRNT        0       /* Print Jmesh warning messages */
//end added by markos

/*
** -----------------------------------------------------------------
** Public Function:
*/

#ifdef NO_PROTOTYPE

int POct3DGeneration( ) ;
int PMsh3DGeneration( ) ;
/*int Msh3DInternalNodes( ) ;
double Msh3DOptimalNodes( ) ;
void Msh3DError( ) ;
void Msh3DBdryReset( ) ;
Msh3DBdryFace Msh3DBdryNext( ) ;*/
int PMsh3DBdryContraction( ) ;
void PMsh3DFaceFreeAll( ) ;
/*int ExtractVert( ) ;
int ExtractEdge( ) ;
int ExtractFace( ) ;

//added by Markos
Msh3DOctTree Msh3DTree( ) ;
Box3d Msh3DTreeBox( ) ;
//end added by Markos*/

#else

int POct3DGeneration( int ver, int oct, double fac, double tol,
                     int offset, int num_node, int num_face,
		     double **nodes, int *tnodes, int **faces,
		     int *num_gen_nodes, double **generated_nodes,
		     int **tgenerated_nodes, int *num_elems, int **elems,
		     int pjmesh_oct_refi, double box_min[3], double box_max[3] ) ;
int PMsh3DGeneration( int ver, int oct, double fac, double tol,
                     int offset, int num_node, int num_face,
		     double **nodes, int *tnodes, int **faces,
		     int *num_gen_nodes, double **generated_nodes,
		     int **tgenerated_nodes, int *num_elems, int **elems,
		     int pjmesh_num_original_elements, int pjmesh_num_original_faces,
             int *pjmesh_original_faces, int pjmesh_use_box_test,
             double *pjmesh_box_min, double *pjmesh_box_max,
             double pjmesh_range_proportion ) ;
/*int Msh3DInternalNodes( int num_org_nodes, int num_org_faces,
                        double **original_nodes, int **original_faces,
                        int *num_gen_nodes, double **generated_nodes ) ;
double Msh3DOptimalNodes( double *face_center,
                          double *tree_center,
                          int  *tree_level ) ;
void Msh3DError( int *number, char **text ) ;
void Msh3DBdryReset( ) ;
Msh3DBdryFace Msh3DBdryNext( ) ;*/
int PMsh3DBdryContraction( int version, int offset, int num_org_nodes,
                          int num_org_faces, double **original_nodes,
                          int *toriginal_nodes, int **original_faces,
                          int *num_int_nodes, double **internal_nodes,
                          int **tinternal_nodes, int *num_gen_elements,
                          int **generated_elements,
                          int pjmesh_num_original_elements, int pjmesh_num_original_faces,
						  int *pjmesh_original_faces, int pjmesh_use_box_test,
						  double *pjmesh_box_min, double *pjmesh_box_max,
						  double pjmesh_range_proportion ) ;
void PMsh3DFaceFreeAll( ) ;
/*int ExtractVert( int nnodes, Msh3DBdryNodeList node_list,
                 int *v1, int *v2, int *v3) ;
int ExtractEdge( Msh3DBdryNodeList node_list, Msh3DBdryFace *f1,
                 Msh3DBdryFace *f2, int *v1, int *v2 ) ;
int ExtractFace( Msh3DBdryNodeList node_list, Msh3DBdryFace *f1,
                 int *v1, int *num_nodes );

//added by Markos
Msh3DOctTree Msh3DTree( ) ;
Box3d Msh3DTreeBox( ) ;
//end added by Markos*/

#endif

#ifdef __cplusplus
	}
}
#endif

#endif //#ifndef _PJMESH_MAIN_DEF3D_H_
