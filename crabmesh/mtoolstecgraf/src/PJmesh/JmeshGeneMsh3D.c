/*
** ---------------------------------------------------------------
**
** JmeshGeneMsh3D.c - Main driver to generate 3D mesh.
**
** ---------------------------------------------------------------
**
** Description:
**  This file implements a driver to generate 3D meshes for any
**  arbitray regions, including regions with one or multiple
**  cracks. The algorithm assumes that a correct surface mesh
**  is given, otherwise possible meshing problems might happen.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**  Modified:	Dec-09-1999	Joaquim Bento Cavalcante Neto
**   Changed variable internal by oct, factor by fac, and created
**   a new variable ver to define the jmesh version to be used.
**  Modified:	Mar-26-2003     Joaquim Bento Cavalcante Neto
**   Included procedure to change the version (from ADVAN to SMALL
**   or vice-versa) and try to generate the mesh again, in case
**   it failed in the first try. If still fails in the second time
**   than it must be a problem in the input and then return. Also
**   created #defines VER_BOTH and VER_PRNT for these procedures.
**   Created #define VER_USED to show the version used by Jmesh.
**  Modified:   Oct-23-2003     Joaquim Bento Cavalcante Neto
**   Modified #define VER_USED by DIA_USED and created DIA_STAT.
**   Modified to get rid of all remaining compiler's warnings.
**
** ---------------------------------------------------------------
**
*/

#include "PJmesh/JmeshMainDef3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//commented by Markos
//#define SMALL_VERSION   0
//#define ADVAN_VERSION   1
//end commented by Markos

/* --------------------------------------------------------------
 * ** Definitons for use of algorithms:
 * */

#ifdef VER_PRNT
#undef VER_PRNT
#endif //#ifdef VER_PRNT

#define VER_BOTH        1   /* Try   Jmesh with two versions */
#define VER_PRNT        1   /* Print Jmesh  warning messages */

#define DIA_USED        0   /* Diagnosis for version used */
#define DIA_STAT        0   /* Diagnosis for mesh  status */

/* --------------------------------------------------------------
** Global Variables:
*/

int    int_octree ;
double ref_factor ;
double tol_factor ;
double tol_inters ;
int    gbl_quit ;

/* --------------------------------------------------------------
** Public function:
*/

int POct3DGeneration(
 int	ver,
 int	oct,
 double fac,
 double	tol,
 int    offset,
 int	num_node,
 int	num_face,
 double **nodes,
 int    *tnodes,
 int    **faces,
 int    *num_gen_nodes,
 double **generated_nodes,
 int    **tgenerated_nodes,
 int    *num_elems,
 int    **elems,
 int    pjmesh_oct_refi, /**  uniform refinement in octree (or not) */
 double box_min[3],		/**  box_min and box_max limit the bounding box for */
 double box_max[3] )		/**  mesh generation                                */
{
    int  status;
    //int pjmesh_oct_refi = 1;

    /* 0.1 Initialize global parameters to generate the mesh */

    int_octree = oct ;
    ref_factor = fac ;
    tol_factor = tol ;

    /* 0.2 Generate 3D octree if wanted */

    if( int_octree )
     status = Msh3DInternalNodes( num_node, num_face, nodes, faces,
       num_gen_nodes, generated_nodes, pjmesh_oct_refi ) ;
    else
     *num_gen_nodes = 0 ;

	return status;
}

int PMsh3DGeneration(
 int	ver,
 int	oct,
 double fac,
 double	tol,
 int    offset,
 int	num_node,
 int	num_face,
 double **nodes,
 int    *tnodes,
 int    **faces,
 int    *num_gen_nodes,
 double **generated_nodes,
 int    **tgenerated_nodes,
 int    *num_elems,
 int    **elems,
 int    pjmesh_num_original_elements, /** number of elements originally in mesh */
 int    pjmesh_num_original_faces, /** number of faces in boundary */
 int    *pjmesh_original_faces, /** faces in boundary */
 int    pjmesh_use_box_test, /** will the box test be used (slave process)? */
 double *pjmesh_box_min,     /** box_min and box_max limit the bounding box for */
 double *pjmesh_box_max,     /**      mesh generation when the box test is used */
 double pjmesh_range_proportion ) /** range_proportion determines how much of the range search will be used for a restriction in parallel meshing */
 {
    int  status, nodeid, i, j ;
    char tver[80] ;

    /* 0.1 Initialize global parameters to generate the mesh */

    /*int_octree = oct ;
    ref_factor = fac ;
    tol_factor = tol ;*/

    /* 0.2 Generate 3D octree if wanted */

    /*if( int_octree )
     status = Msh3DInternalNodes( num_node, num_face, nodes, faces,
       num_gen_nodes, generated_nodes ) ;
    else
     *num_gen_nodes = 0 ;*/

    /* 0.3 Generate 3D mesh by Boundary Contraction operations */

    status = PMsh3DBdryContraction( ver, offset, num_node, num_face, nodes,
      tnodes, faces, num_gen_nodes, generated_nodes, tgenerated_nodes,
      num_elems, elems,
      pjmesh_num_original_elements, pjmesh_num_original_faces,
	  pjmesh_original_faces, pjmesh_use_box_test,
	  pjmesh_box_min, pjmesh_box_max, pjmesh_range_proportion ) ;

#if DIA_STAT
    fprintf (stderr, "\t                Mesh status:\n" ) ;
    fprintf (stderr, "\t\t        %d\n",  status) ;
#endif


    /* 0.4 If fails, change the version and try to generate 3D mesh by
           Boundary Contraction operations again */

#if VER_BOTH
    switch (status)
    {
     case 0:                   /* Try a different version */
     /* Change version */
     if (ver == ADVAN_VERSION) ver = SMALL_VERSION ;
     else                      ver = ADVAN_VERSION ;
     /* Print message to user */
#if VER_PRNT
     fprintf (stderr, "\n" );
     fprintf (stderr, "\n" );
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\t                  J M E S H                       \n");
     fprintf (stderr, "\tWARNING:  The volume mesh could not be generated!!\n");
     fprintf (stderr, "\tIt is probably related  to problems in the surface\n");
     fprintf (stderr, "\tmesh,  regarding the input requirements for JMESH.\n");
     fprintf (stderr, "\tJMESH will try a new approach, with no guarantees.\n");
     fprintf (stderr, "\tFor more details,  please see JMESH documentation.\n");
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\n" );
#endif
     /* Try Jmesh again with the other version */
     status = PMsh3DBdryContraction( ver, offset, num_node, num_face, nodes,
      tnodes, faces, num_gen_nodes, generated_nodes, tgenerated_nodes,
      num_elems, elems,
      pjmesh_num_original_elements, pjmesh_num_original_faces,
	  pjmesh_original_faces, pjmesh_use_box_test,
	  pjmesh_box_min, pjmesh_box_max, pjmesh_range_proportion ) ;
     break ;
     case 1:                   /* Try a different version not necessary */
     status = 1 ;
     break ;
     case 2:                   /* Do not try a different version */
     status = 0 ;
     break ;
    }
    /* Print used version */
#if DIA_USED
    fprintf (stderr, "\t                Used version:\n" ) ;
    if (ver == ADVAN_VERSION) strcpy (tver, "ADVAN_VERSION") ;
    else                      strcpy (tver, "SMALL_VERSION") ;
    fprintf (stderr, "\t\t        %s\n",  tver) ;
    printf ("\t                Used version:\n" ) ;
    printf ("\t\t        %s\n",  tver) ;
    fprintf (stderr, "\n" ) ;
#endif
    /* Return status for generation */
    return status ;
#else
    /* Return status for generation */
    return status ;
#endif
}

