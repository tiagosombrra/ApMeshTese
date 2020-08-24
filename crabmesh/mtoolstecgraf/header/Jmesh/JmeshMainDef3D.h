#ifndef _JMESH_MAIN_DEF3D_H_
#define _JMESH_MAIN_DEF3D_H_
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

#ifdef __cplusplus
extern "C" {
#endif

#define Msh3DMalloc  malloc
#define Msh3DCalloc  calloc
#define Msh3DRealloc realloc
#define Msh3DFree    free

//added by Markos
#define SMALL_VERSION 	0
#define ADVAN_VERSION 	1
//end added by Markos

/*
** -----------------------------------------------------------------
** Public Types:
*/

typedef struct _Msh3DBdryFaceRec {
  struct _Msh3DBdryFaceRec  *next ; /* next face pointer       */
  struct _Msh3DBdryFaceRec  *prev ; /* previous face pointer   */
  int     verts[3] ;                /* corner vertices         */
  int     use ;                     /* new node use status     */
  int     layer ;                   /* layer of the face       */
  double  center[3] ;               /* center point coordinate */
  double  max[3] ;                  /* max x, y, and z         */
  double  min[3] ;                  /* min x, y, and z         */
  double  r[3] ;                    /* vector from v1 to v2    */
  double  s[3] ;                    /* vector from v1 to v3    */
  double  t[3] ;                    /* vector from v2 to v3    */
  double  r_x_s[3] ;                /* face normal             */
  double  rs_dot_v1 ;               /* (r x s) * v1            */
  double  area ;                    /* area of the triangle    */
  double  key ;                     /* key value for sort      */
  int     adtreeindx ;              /* index in AD tree        */
  int     rgn[2] ;                  /* elements associated     */
}
Msh3DBdryFaceRec, *Msh3DBdryFace, *Msh3DBdryStack ;

typedef struct _Msh3DPolyFaceRec {
    struct _Msh3DPolyFaceRec *next ; /* next pointer (or null)  */
    struct _Msh3DPolyFaceRec *prev ; /* prev pointer (or null)  */
    Msh3DBdryFace             face ; /* address of the face     */
}
Msh3DPolyFaceRec, *Msh3DPolyFace, *Msh3DPolyFaceList ;

typedef struct _Msh3DAdjFaceRec {
    struct _Msh3DAdjFaceRec *next ; /* next pointer (or null)  */
    Msh3DBdryFace            face ; /* address of the face     */
}
Msh3DAdjFaceRec, *Msh3DAdjFace, *Msh3DAdjFaceList ;

typedef struct _Msh3DAdjIniFaceRec {
    struct _Msh3DAdjIniFaceRec *next ;     /* next pointer (or null)         */
    int                         verts[3] ; /* verts of initial adjacent face */
    double                      r_x_s[3] ; /* r_x_s of initial adjacent face */
}
Msh3DAdjIniFaceRec, *Msh3DAdjIniFace, *Msh3DAdjIniFaceList ;

typedef struct _Msh3DAdjElemRec {
    struct _Msh3DAdjElemRec *next ;     /* next pointer (or null)  */
    int                      elem ;     /* address  of the element */
    int                      verts[4] ; /* vertices of the element */
} Msh3DAdjElemRec, *Msh3DAdjElem, *Msh3DAdjElemList ;

typedef struct _Msh3DBdryNodeRec {
    int                  real_id ;     /* node id                    */
    double               coord[3] ;    /* nodal coordinates          */
    int                  mates[4] ;    /* mate nodes                 */
    int                  active_flag ; /* active node flag           */
    Msh3DAdjElemList     elems ;       /* adjacent elem list         */
    Msh3DAdjFaceList     faces ;       /* adjacent face list         */
    Msh3DAdjIniFaceList  ifaces ;      /* adjacent initial face list */
}
Msh3DBdryNodeRec, *Msh3DBdryNode, *Msh3DBdryNodeList ;

typedef enum {
  GDB_VERTEX,
  GDB_EDGE,
  GDB_FACE,
  GDB_NEWFACE,
  GDB_DELFACE,
  GDB_TREE
} Top;

//added by Markos
typedef struct _Msh3DOctTreeRec {
  struct _Msh3DOctTreeRec  *child[8] ;
  struct _Msh3DOctTreeRec  *parent ;
  int                       flags ;
  int                       marks ;
}
Msh3DOctTreeRec, *Msh3DOctTree ;

typedef struct _Box3d {
  double min[3], max[3] ;
}
Box3d ;
//end added by Markos

/*
** -----------------------------------------------------------------
** Public Variables:
*/

extern  Msh3DBdryNodeList  node_list ;
extern  Msh3DBdryFace      bdry_stack ;
extern  Msh3DPolyFace      poly_stack ;
extern  int                fase_stack ;
extern  int                nintnode ;
extern  int                nbdrynode ;
extern  int                gbl_quit ;
extern  int                GdbWait ;
extern  int		   DebugFail ;
extern  double             ref_factor ;
extern  double             tol_factor ;
extern  double             tol_inters ;
extern  double             gmin[3], gmax[3] ;
extern  int                int_octree ;
extern  int                int_offset ;

extern Msh3DBdryFace      bdry_free ;
extern Msh3DBdryFace      bdry_block_ptr ;
extern Msh3DBdryFace      bdry_tail ;
extern Msh3DBdryFace      bdry_last ;
extern Msh3DBdryFace      bdry_cursor ;
extern Msh3DBdryFace      bdry_try ;
extern Msh3DBdryFace      bdry_use1 ;
extern Msh3DBdryFace      bdry_use2 ;

extern Msh3DPolyFace      poly_free ;
extern Msh3DPolyFace      poly_block_ptr ;
extern Msh3DPolyFace      poly_tail ;
extern Msh3DPolyFace      poly_cursor ;

//added by markos
extern int useUniformLevel ;
extern int uniformLevel ;
//end added by markos

/*
** -----------------------------------------------------------------
** Public Function:
*/

#ifdef NO_PROTOTYPE

int Msh3DGeneration( ) ;
int Msh3DInternalNodes( ) ;
double Msh3DOptimalNodes( ) ;
void Msh3DError( ) ;
void Msh3DBdryReset( ) ;
Msh3DBdryFace Msh3DBdryNext( ) ;
int Msh3DBdryContraction( ) ;
int ExtractVert( ) ;
int ExtractEdge( ) ;
int ExtractFace( ) ;

//added by Markos
Msh3DOctTree Msh3DTree( ) ;
Box3d Msh3DTreeBox( ) ;
void Msh3DPolyReset() ;
Msh3DPolyFace Msh3DPolyNext() ;
//end added by Markos

#else

int Msh3DGeneration( int ver, int oct, double fac, double tol,
                     int offset, int num_node, int num_face,
		     double **nodes, int *tnodes, int **faces,
		     int *num_gen_nodes, double **generated_nodes,
		     int **tgenerated_nodes, int *num_elems, int **elems,
		     int pjmesh_oct_refi, void (*callback_advanced_faces_ptr)(int, int) ) ; //added by markos
int Msh3DInternalNodes( int num_org_nodes, int num_org_faces,
                        double **original_nodes, int **original_faces,
                        int *num_gen_nodes, double **generated_nodes,
						int pjmesh_oct_refi ) ;
double Msh3DOptimalNodes( double *face_center,
                          double *tree_center,
                          int  *tree_level ) ;
void Msh3DError( int *number, char **text ) ;
void Msh3DBdryReset( ) ;
Msh3DBdryFace Msh3DBdryNext( ) ;
int Msh3DBdryContraction( int version, int offset, int num_org_nodes,
                          int num_org_faces, double **original_nodes,
                          int *toriginal_nodes, int **original_faces,
                          int *num_int_nodes, double **internal_nodes,
                          int **tinternal_nodes, int *num_gen_elements,
                          int **generated_elements,
                          void (*callback_advanced_faces_ptr)(int, int) ) ; //added by markos
int ExtractVert( int nnodes, Msh3DBdryNodeList node_list,
                 int *v1, int *v2, int *v3) ;
int ExtractEdge( Msh3DBdryNodeList node_list, Msh3DBdryFace *f1,
                 Msh3DBdryFace *f2, int *v1, int *v2 ) ;
int ExtractFace( Msh3DBdryNodeList node_list, Msh3DBdryFace *f1,
                 int *v1, int *num_nodes );

//added by Markos
Msh3DOctTree Msh3DTree( ) ;
Box3d Msh3DTreeBox( ) ;
void Msh3DPolyReset() ;
Msh3DPolyFace Msh3DPolyNext() ;
//end added by Markos

#endif

#ifdef __cplusplus
}
#endif

#endif //#ifndef _JMESH_MAIN_DEF3D_H_
