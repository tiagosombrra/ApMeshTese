/*
** -----------------------------------------------------------------
**
** JmeshBdryOct3D.c - Octree generation module.
**
** -----------------------------------------------------------------
**
** Description:
**  Package for implementing usual 3D octree operations based on
**  the boundary defined by triangular facets given as an input.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**   It was based on Wash's implementation for octree operations.
**  Modified:	Oct-25-2001	Joaquim Bento Cavalcante Neto
**   Changed the use of variable fac to allow more control over the
**   degree of refinement of the mesh.
**  Modified:	Mar-26-2003	Joaquim Bento Cavalcante Neto
**   Created #defines starting with OCT_ for use of algorithms.
**   Also deleted #define BDR_OCTREE that was changed by OCT_FREE.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified variable bound_min_level to init as 1000000 instead
**   of INFPOS. Also changed function Msh3DOctSize to retun calling
**   (return Msh3DOctSize) instead of only (Msh3DOctSize) to avoid
**   compilers' warnings related to returns for all control paths.
**   Finally, modified to get rid of remaining compiler's warnings.
**  Modified:	Jan-16-2004	Joaquim Bento Cavalcante Neto
**   Modified function Msh3DOctDNode to pass variables num_gen_nodes
**   and generated_nodes to avoid Jmesh to crash.
**
** -----------------------------------------------------------------
**
*/

#include "Jmesh/JmeshMainDef3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* --------------------------------------------------------------
** Definitons for use of algorithms:
*/

#define OCT_REFI	0	/* Refine octree for largest cell   */
#define OCT_MARK	0	/* Mark cells near given boundary   */
#define OCT_CLAS	0	/* Classify all cells of the octree */
#define OCT_GENE	0	/* Generate nodes at unmarked cells */
#define OCT_DISP	0	/* Print octree cells for display   */
#define OCT_FREE	0	/* Release memory for octree cells  */

/* ----------------------------------------------------------------
** Private definitons and data types:
*/

#define REF_FACTOR     0.4       /* changed by ref_factor */
#define INT_OCTREE     1         /* changed by int_octree */
#define NEW_NODES      10
#define INFPOS         1e+06
#define INFNEG        -1e+06
#define PI             3.141592653589793324
#define MAX(a,b)       (((a)>(b))?(a):(b))
#define MIN(a,b)       (((a)<(b))?(a):(b))

/*//commented by Markos
//moved to JmeshMainDef3D.h
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
//end commented by Markos*/

/* --------------------------------------------------------------
** Global variables:
*/

double gmin[3], gmax[3] ;
double bmin[3], bmax[3] ;

/* --------------------------------------------------------------
** Private variables:
*/

static Msh3DOctTree root = 0 ;
static Box3d  box ;
//added by markos
int useUniformLevel = 1;
int uniformLevel = 0;
//end added by markos

/* --------------------------------------------------------------
** Private functions prototypes:
*/

static Msh3DOctTree Msh3DBdryOct( Msh3DOctTree, Msh3DOctTree, double,
                                  double [3], double [3], double [3],
				  double, int, int * ) ;
static Msh3DOctTree Msh3DBdryOctFull( Msh3DOctTree, Msh3DOctTree, double,
	                              double [3], double [3], double [3],
				      double, int, int *, int, int ) ;
static Msh3DOctTree Msh3DRefOct( Msh3DOctTree, Msh3DOctTree, int, int ) ;
static void         Msh3DTransOct( Msh3DOctTree, int ) ;
static int          Msh3DOctNeighbor( Msh3DOctTree, int, int, Msh3DOctTree *,
	                              int *, int * ) ;
static void         Msh3DOctMark( Msh3DOctTree, double [3], double [3],
		                  double [3] ) ;
static int          Msh3DOctSize( Msh3DOctTree, double [3], double [3],
		                  double [3], double [3], int, double * ) ;
static void         Msh3DOctClass( Msh3DOctTree, int, int, double **, int **,
		                   double [3], double [3] ) ;
static void         Msh3DOctClassFull( Msh3DOctTree, int, int, double **,
		                       int **, double [3], double [3] ) ;
static double       Msh3DOctClassSolidAngle( double **, int, int, int,
		                             double [3] ) ;
static double       Msh3DOctClassSolidEdge( double [3], int, int, double [3],
		                            double [3], double [3], double [3] ) ;
static void         Msh3DOctClassVolum( double [3][3], double [3], double [3],
		                        double *, double * ) ;
static double       Msh3DOctClassVolumFace( double [4][3], int, int [3] ) ;
static void         Msh3DOctGenNodes( Msh3DOctTree, double [3], double [3],
		                      int *, double ** );
static void         Msh3DOctGenNodesFull( Msh3DOctTree, double [3], double [3],
		                          int *, double ** );
static Msh3DOctTree Msh3DOctAlloc( void ) ;
static void         Msh3DOctFreeAll( void ) ;
static void         Msh3DOctFree( Msh3DOctTree ) ;
static void         Msh3DOctDTree( Msh3DOctTree, double [3], double [3] ) ;
static void         Msh3DOctDBound( double [3], double [3] ) ;
static void         Msh3DOctDNodes( int *, double ** ) ;

/* -------------------------------------------------------------------
** Msh3DInternalNodes - find the location of internal nodes.
*/

int Msh3DInternalNodes(
    int     num_org_nodes,
    int     num_org_faces,
    double  **original_nodes,
    int     **original_faces,
    int     *num_gen_nodes,
    double  **generated_nodes,
    int     pjmesh_oct_refi )
{
    double max[3], min[3], mid[3], span[3] ;
    double coords[3][3], cross[3] ;
    double area ;
    int    i, j, k, min_level, bound_min_level = 1000000 ;
    Msh3DOctTree tree = 0 ;

    /* 1.1 Find the max and min x, y, and z coordinates.  Determine the
           smallest cube that will contain all the nodes, and center it
           on the domain */

    for ( i=0 ; i<3 ; i++ ) min[i] = max[i] = original_nodes[0][i] ;
    for ( j=0 ; j<num_org_nodes ; j++ ) {
        for ( i=0 ; i<3 ; i++ ) {
            max[i] = (max[i] > original_nodes[j][i]) ?
                     max[i] : original_nodes[j][i] ;
            min[i] = (min[i] < original_nodes[j][i]) ?
                     min[i] : original_nodes[j][i] ;
        }
    }
    for ( i=0 ; i<3 ; i++ ) {
        bmin[i] = min[i] ;
        bmax[i] = max[i] ;
    }
    for ( i=0 ; i<3 ; i++ ) {
        mid[i] = (max[i]+min[i]) / 2.0 ;
        span[i] = max[i] - min[i] ;
    }
    if (( span[0] >= span[1] ) && ( span[0] >= span[2] )) {
        min[1] = mid[1] - (span[0]/2.0) ;
        max[1] = mid[1] + (span[0]/2.0) ;
        min[2] = mid[2] - (span[0]/2.0) ;
        max[2] = mid[2] + (span[0]/2.0) ;
    }
    else if (( span[1] >= span[0] ) && ( span[1] >= span[2] )) {
        min[0] = mid[0] - (span[1]/2.0) ;
        max[0] = mid[0] + (span[1]/2.0) ;
        min[2] = mid[2] - (span[1]/2.0) ;
        max[2] = mid[2] + (span[1]/2.0) ;
    }
    else {
        min[0] = mid[0] - (span[2]/2.0) ;
        max[0] = mid[0] + (span[2]/2.0) ;
        min[1] = mid[1] - (span[2]/2.0) ;
        max[1] = mid[1] + (span[2]/2.0) ;
    }

    /* fill in box structure */

    for ( i=0 ; i<3 ; i++ ) {
       //added by markos
       span[i] = max[i] - min[i];
       min[i] -= 0.001*span[i];
       max[i] += 0.001*span[i];
       //end added by markos
       box.min[i] = gmin[i] = min[i];
       box.max[i] = gmax[i] = max[i];
    }

    /* 1.2 For each face in the boundary, find the mid-point of the face
           and it's area.  Locate the mid-point in the octree and look
           at the cell size.  If the cell size is approximately equal
           to or smaller than the face size, go on to the next face.
           If not, subdivide the octant. */

    for ( i=0 ; i<num_org_faces ; i++ ) {
//fprintf (stderr,"markos debug, generating tree, face = %d, %d, %d\n", original_faces[i][0], original_faces[i][1], original_faces[i][2]);
        for ( j=0 ; j<3 ; j++ ) {
            for ( k=0 ; k<3 ; k++ ) {
                coords[j][k] = original_nodes[original_faces[i][j]][k] ;
            }
//fprintf (stderr,"markos debug, generating tree, face->v%d = %f, %f, %f\n", j+1, coords[j][0], coords[j][1], coords[j][2]);
        }

        /* compute the center */

        for ( j=0 ; j<3 ; j++ ) {
            mid[j] = 0.0 ;
            for ( k=0 ; k<3 ; k++ ) mid[j] += coords[k][j] ;
            mid[j] /= 3.0 ;
        }

//fprintf (stderr,"markos debug, generating tree, mid = %f, %f, %f\n", mid[0], mid[1], mid[2]);

        /* the area is |p0 x p1 + p1 x p2 + p2 x p0|/2 */

        cross[0] = coords[0][1]*coords[1][2] - coords[1][1]*coords[0][2] ;
        cross[1] = coords[0][2]*coords[1][0] - coords[1][2]*coords[0][0] ;
        cross[2] = coords[0][0]*coords[1][1] - coords[1][0]*coords[0][1] ;

        cross[0] += coords[1][1]*coords[2][2] - coords[2][1]*coords[1][2] ;
        cross[1] += coords[1][2]*coords[2][0] - coords[2][2]*coords[1][0] ;
        cross[2] += coords[1][0]*coords[2][1] - coords[2][0]*coords[1][1] ;

        cross[0] += coords[2][1]*coords[0][2] - coords[0][1]*coords[2][2] ;
        cross[1] += coords[2][2]*coords[0][0] - coords[0][2]*coords[2][0] ;
        cross[2] += coords[2][0]*coords[0][1] - coords[0][0]*coords[2][1] ;

        area = sqrt( cross[0]*cross[0] + cross[1]*cross[1] +
                     cross[2]*cross[2] ) / 2.0 ;

//fprintf (stderr,"markos debug, generating tree, area = %f\n", area);

        min_level = -1 ;
        tree = Msh3DBdryOct( tree, 0, area, mid, max, min, ref_factor,
                   1, &min_level ) ;
        if( min_level < bound_min_level ) bound_min_level = min_level ;
    }
    if ( !tree ) {
        tree = Msh3DOctAlloc() ;
        tree->parent = 0 ;
        tree->marks  = 0 ;
    }

    /* 1.3 Find the largest cell size that contains a boundary face.
           Refine the entire octree to at least this level. */

//#if OCT_REFI
	uniformLevel = 0;
	if (pjmesh_oct_refi)
	{
//fprintf (stderr,"markos debug, generating tree, min_level = %d\n", bound_min_level);

		tree = Msh3DRefOct( tree, 0, 1, bound_min_level ) ;
	}
//#endif

    /* 1.4 Refine the octree so that no adjacent cells differ by a
           refinement factor of more than one. */

    Msh3DTransOct( tree, 0 ) ;

    /* 1.5 Mark all cells that contain a boundary vertex or the center
	   of a boundary face. */

#if OCT_MARK
    for ( i=0 ; i<num_org_nodes ; i++ )
        Msh3DOctMark( tree, max, min, &original_nodes[i][0] ) ;

    for ( i=0 ; i<num_org_faces ; i++ ) {
        for ( j=0 ; j<3 ; j++ ) {
            for ( k=0 ; k<3 ; k++ ) {
                coords[j][k] = original_nodes[original_faces[i][j]][k] ;
            }
        }

     /* find the center of the face */

        for ( j=0 ; j<3 ; j++ ) {
            mid[j] = 0.0 ;
            for ( k=0 ; k<3 ; k++ ) mid[j] += coords[k][j] ;
            mid[j] /= 3.0 ;
        }
        Msh3DOctMark( tree, max, min, mid ) ;
    }
#endif

    /* 1.6 Classify all cells of the octree. Mark all cells that are in
	   the boundary of the body (cells that falls too close  to  a
	   boundary face) and cells that are outside of the body. */

#if OCT_CLAS
    Msh3DOctClass( tree, num_org_nodes, num_org_faces, original_nodes,
		   original_faces, max, min );
#endif

    /* 1.7  Now generate a node at the center of each unmarked terminal
	    cell (cells that are inside of the body). */

#if OCT_GENE
    Msh3DOctGenNodes( tree, max, min, num_gen_nodes, generated_nodes ) ;
#endif

    /* 1.8  Print all cells for display and release octree memory if
	    desired. Also get the root of the octree generated. */

    Msh3DOctDBound( min, max ) ;
    Msh3DOctDTree( tree, min, max ) ;
    Msh3DOctDNodes( num_gen_nodes, generated_nodes ) ;
    Msh3DOctFreeAll() ;

#if DSP_MESH
    GdbSetCurTreeOn (tree);
#endif

    root = tree ;

    Msh3DOctFree (root) ;

    return(1) ;
}

/* -------------------------------------------------------------------
** Msh3DOptimalNodes - find the size of the cell where the given face
**                     center is to use for the location of optimal nodes.
*/

double Msh3DOptimalNodes(
    double  face_center[3],
    double  tree_center[3],
    int    *tree_level )
{
    int     level ;
    double  size ;

    if( root == NULL ) return 0.0 ;

    /* 1.1 Find the size of the cell where the given point is */

    level = Msh3DOctSize( root, gmax, gmin, face_center, tree_center, 1, &size ) ;
    *tree_level = level ;

    //added by markos
    if ((useUniformLevel) && (uniformLevel > 0) && (level < uniformLevel))
	{
		*tree_level = uniformLevel;
		size = (gmax[0] - gmin[0])/pow(2.0, (double)uniformLevel);
	}
    //end added by markos

    /* 1.2 Return the found size */

    return size ;
}

/* -------------------------------------------------------------------
** Msh3DBdryOct - subdivide the oct tree until the area of the sides
**                of the octant containing the given point is about
**                the same as the area associated with the point.
*/

static Msh3DOctTree Msh3DBdryOct(
    Msh3DOctTree  tree,
    Msh3DOctTree  parent,
    double        area,
    double        coord[3],
    double        max[3],
    double        min[3],
    double        factor,
    int           this_level,
    int           *min_level )
{
    double        len, carea ;
    int           nindx, indx[2], i ;
    double        lmin[2][3], lmax[2][3], mid[3] ;

    /* find the size of the cell and the current length */

    //fprintf (stderr,"markos debug, generating tree, level = %d\n", this_level);

    len = max[0] - min[0] ;  carea = len * len ;

    //fprintf (stderr,"markos debug, generating tree, carea = %f\n", carea);

    if ( area > carea * factor ) {
        if (( *min_level == -1 ) || ((this_level-1) < *min_level ))
            *min_level = this_level-1 ;
        return( tree ) ;
    }

    /* if we get here we decend the tree */

    if ( !tree ) {
        tree = Msh3DOctAlloc() ;
        tree->parent = parent ;
        tree->marks  = 0 ;
    }

    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    if ( coord[0] == mid[0] || coord[1] == mid[1] || coord[2] == mid[2] )
     nindx = 2 ;
    else
     nindx = 1 ;
    indx[0] = indx[1] = 0 ;

    for ( i=0 ; i<3 ; i++ ) {
        if ( coord[i] == mid[i] ) {
            indx[0] |= (1<<i) ; lmin[0][i] = mid[i] ; lmax[0][i] = max[i] ;
            lmin[1][i] = min[i] ; lmax[1][i] = mid[i] ;
        }
        else {
         if ( coord[i] > mid[i] ) {
             indx[0] |= (1<<i) ;  lmin[0][i] = mid[i] ;  lmax[0][i] = max[i] ;
             indx[1] |= (1<<i) ;  lmin[1][i] = mid[i] ;  lmax[1][i] = max[i] ;
         }
         else {
             lmin[0][i] = min[i] ;  lmax[0][i] = mid[i] ;
             lmin[1][i] = min[i] ;  lmax[1][i] = mid[i] ;
         }
       }
    }

    for ( i=0; i<nindx ; i++ )
     tree->child[indx[i]] = Msh3DBdryOct( tree->child[indx[i]], tree, area,
                             coord, lmax[i], lmin[i],
                             factor, this_level+1, min_level ) ;

    return( tree ) ;
}

static Msh3DOctTree Msh3DBdryOctFull(
    Msh3DOctTree  tree,
    Msh3DOctTree  parent,
    double        area,
    double        coord[3],
    double        max[3],
    double        min[3],
    double        factor,
    int           this_level,
    int           *min_level,
    int           this_child,
    int           indx_child )
{
    double        len, carea ;
    //int           nindx, indx[2], i, this ;
    int           nindx, indx[2], i, th ;
    double        lmin[2][3], lmax[2][3], mid[3] ;

    /* alloc a cell of the tree if it's necessary */

    if ( !tree ) {
        tree = Msh3DOctAlloc() ;
        tree->parent = parent ;
    }

    /* return tree if this indx is not the child indx where coord is */

    if( this_child != indx_child )
     return( tree ) ;

    /* find the size of the cell and the current length */

    len = max[0] - min[0] ;  carea = len * len ;

    /* return tree if face's area is bigger than cell's area */

    if ( area > carea * factor ) {
        if (( *min_level == -1 ) || ((this_level-1) < *min_level ))
            *min_level = this_level-1 ;
        return( tree ) ;
    }

    /* if we get here we decend the tree */

    //for( this = 0; this < 8; this++ ) {
    for( th = 0; th < 8; th++ ) {

     for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

     if ( coord[0] == mid[0] || coord[1] == mid[1] || coord[2] == mid[2] )
      nindx = 2 ;
     else
      nindx = 1 ;
     indx[0] = indx[1] = 0 ;

     for ( i=0 ; i<3 ; i++ ) {
         if ( coord[i] == mid[i] ) {
             indx[0] |= (1<<i) ; lmin[0][i] = mid[i] ; lmax[0][i] = max[i] ;
             lmin[1][i] = min[i] ; lmax[1][i] = mid[i] ;
         }
         else {
          if ( coord[i] > mid[i] ) {
              indx[0] |= (1<<i) ;  lmin[0][i] = mid[i] ;  lmax[0][i] = max[i] ;
              indx[1] |= (1<<i) ;  lmin[1][i] = mid[i] ;  lmax[1][i] = max[i] ;
          }
          else {
              lmin[0][i] = min[i] ;  lmax[0][i] = mid[i] ;
              lmin[1][i] = min[i] ;  lmax[1][i] = mid[i] ;
          }
        }
     }

     for ( i=0; i<nindx ; i++ )
      //tree->child[this] = Msh3DBdryOctFull( tree->child[this], tree, area,
      tree->child[th] = Msh3DBdryOctFull( tree->child[th], tree, area,
                             coord, lmax[i], lmin[i],
                             //factor, this_level+1, min_level, this, indx[i] ) ;
                             factor, this_level+1, min_level, th, indx[i] ) ;
    }
    return( tree ) ;
}

/* -------------------------------------------------------------------
** Msh3DRefOct - subdivide the oct tree to at least the specified level.
*/

static Msh3DOctTree Msh3DRefOct(
    Msh3DOctTree  tree,
    Msh3DOctTree  parent,
    int           this_level,
    int           min_level )
{
    int j ;

    /* fprintf( stderr, "Min level is : %d\n", min_level ) ; */

    if ( this_level > min_level ) return( tree ) ;

    if ( !tree ) {

	/* fprintf( stderr, "Refine the tree\n" ); */

        tree = Msh3DOctAlloc() ;
        tree->parent = parent ;
    }

    //added by markos
    if (useUniformLevel)
	{
		uniformLevel = min_level;

		return tree;
	}
    //end added by markos

    for ( j=0 ; j<8 ; j++ ) {
        tree->child[j] =
            Msh3DRefOct( tree->child[j], tree, this_level+1, min_level ) ;
    }
    return( tree ) ;
}

/* -------------------------------------------------------------------
** Msh3DTransOct - subdivide the oct tree so that no two neighbor cells
**                 differ in level by more than one.
*/

static int direct_tbl[8][3] = {
    { 3, 4, 5 },
    { 0, 4, 5 },
    { 3, 1, 5 },
    { 0, 1, 5 },
    { 3, 4, 2 },
    { 0, 4, 2 },
    { 3, 1, 2 },
    { 0, 1, 2 },
    } ;

static void Msh3DTransOct( Msh3DOctTree tree, int level )
{
    int i, j, direction, clevel, nchild ;
    Msh3DOctTree  neighbor ;

    for ( j=0 ; j<8 ; j++ ) {
        if ( tree->child[j] ) {
            Msh3DTransOct( tree->child[j], level+1 ) ;
        }
        else {
            for ( i=0 ; i<3 ; i++ ) {

                direction = direct_tbl[j][i] ;

                clevel = level - 1 ;
                Msh3DOctNeighbor( tree, j, direction, &neighbor,
                                         &nchild, &clevel ) ;

                /* if the neighbors differ by more than one, subdivide */

                if ( neighbor && ((level-clevel) > 1) ) {

                    /* fprintf(stderr, "Subdivide %d\n", level-clevel ) ; */

                    neighbor->child[nchild] = Msh3DOctAlloc() ;
                    neighbor->child[nchild]->parent = neighbor ;

                    Msh3DTransOct( neighbor->child[nchild], clevel+1 ) ;
                }
            }
        }
    }
}

/* -------------------------------------------------------------------
** Msh3DOctNeighbor - given a node in the octree along with a search
**                    direction, this routine finds the neighbor cell
**                    in the oct tree.
**
** Hang on, this algorithm is not for the faint at heart.  It is an
** extenstion of the quadtree algorithm described by Samet in his
** comp surveys review.  Here, however, I play a bunch of bit tricks
** to make the code managable, and to eliminate a bunch of case
** statements.
**
** The basic idea is this, a cells index tells the location of a cell
** in its parent based on the bit pattern in the cell.  Each index
** is comprised of 3 bits, and each bit codes for a particular direction.
** The low order (right most) bit is for x, 2nd lowest for y, and
** third lowest for z.  If we assume a local coordinate system with
** its origin at the center of the parent cell, then a set bit implies
** that the coordinate values for the corresponding direction (x,y,z)
** have a positive value.  Likewise, a cleared bit indicates negative
** values.
**
** When searching for a neighbor, we need to indicate a search
** direction.  This is done with another set of bit patterns which
** allow us to do some simple bit operations.  The patterns are
** as follows:
**     +x -> 000 (1), -x -> 011 (6), +y -> 001 (2), -y -> 100 (5),
**     +z -> 010 (4), -z -> 101 (3)
*/

static int dir_tbl[8][6] = {
    { 0, 0, 0, 1, 1, 1 },
    { 1, 0, 0, 0, 1, 1 },
    { 0, 1, 0, 1, 0, 1 },
    { 1, 1, 0, 0, 0, 1 },
    { 0, 0, 1, 1, 1, 0 },
    { 1, 0, 1, 0, 1, 0 },
    { 0, 1, 1, 1, 0, 0 },
    { 1, 1, 1, 0, 0, 0 },
    } ;

static int mirror_tbl[8][6] = {
    { 1, 2, 4, 1, 2, 4 },
    { 0, 3, 5, 0, 3, 5 },
    { 3, 0, 6, 3, 0, 6 },
    { 2, 1, 7, 2, 1, 7 },
    { 5, 6, 0, 5, 6, 0 },
    { 4, 7, 1, 4, 7, 1 },
    { 7, 4, 2, 7, 4, 2 },
    { 6, 5, 3, 6, 5, 3 },
    } ;

static int Msh3DOctNeighbor(
    Msh3DOctTree  cell,
    int           child,
    int           direction,
    Msh3DOctTree  *cousin,
    int           *cchild,
    int           *level )
{
    Msh3DOctTree  parent ;
    int           indx, found = 0 ;

    /* first move up the tree until we find a common ancester
       we do this by finding the first time we find a decendent
       in the direction opposite the search direction */

    parent = cell->parent ;
    *level -= 1 ;
    if ( !parent ) {
        *cousin = 0 ;
        return(1) ;
    }
    for ( indx=0 ; indx<8 ; indx++ ) {
        if ( parent->child[indx] == cell ) break ;
    }
    if ( dir_tbl[indx][direction] ) {
        found = Msh3DOctNeighbor( parent, indx, direction, cousin, cchild, level ) ;
    }
    else {
        *cousin = parent ;
        *cchild = mirror_tbl[indx][direction] ;
        *level += 1 ;
    }

    /* decend the tree in the mirror direction */

    if ( !found ) {
        child = mirror_tbl[indx][direction] ;
        if ( (*cousin)->child[child] ) {
            *level += 1 ;
            *cchild = child ;
            *cousin = (*cousin)->child[child] ;
            return ( 0 ) ;
        }
        else
            *cchild = child ;
    }

    return ( 1 ) ;
}

/* -------------------------------------------------------------------
** Msh3DOctMark - given the coordinates of a point, this routine finds
**                the terminal cell that contains the point, and marks
**                the appropriate bit in the flag word.
*/

static void Msh3DOctMark( Msh3DOctTree tree, double max[3], double min[3],
		          double coord[3] )
{
    int           nindx, indx[2], i ;
    double        lmin[2][3], lmax[2][3], mid[3] ;

    /* find the mid point and the appropriate child cell */

    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    if ( coord[0] == mid[0] || coord[1] == mid[1] || coord[2] == mid[2] )
     nindx = 2 ;
    else
     nindx = 1 ;
    indx[0] = indx[1] = 0 ;

    for ( i=0 ; i<3 ; i++ ) {
        if ( coord[i] == mid[i] ) {
            indx[0] |= (1<<i) ; lmin[0][i] = mid[i] ; lmax[0][i] = max[i] ;
            lmin[1][i] = min[i] ; lmax[1][i] = mid[i] ;
        }
        else {
         if ( coord[i] > mid[i] ) {
             indx[0] |= (1<<i) ;  lmin[0][i] = mid[i] ;  lmax[0][i] = max[i] ;
             indx[1] |= (1<<i) ;  lmin[1][i] = mid[i] ;  lmax[1][i] = max[i] ;
         }
         else {
             lmin[0][i] = min[i] ;  lmax[0][i] = mid[i] ;
             lmin[1][i] = min[i] ;  lmax[1][i] = mid[i] ;
         }
       }
    }

    /* check to see if the cell has children.  If so, decend the tree.
       If not, set it's flag */

    for ( i=0; i<nindx ; i++ ) {
     if ( tree->child[indx[i]] ) {
         Msh3DOctMark( tree->child[indx[i]], lmax[i], lmin[i], coord ) ;
     }
     else {
        tree->flags |= (1<<indx[i]) ;
     }
    }
}

#if 0
static void Msh3DOctMark( tree, max, min, coord )
    Msh3DOctTree  tree ;
    double        coord[3], max[3], min[3] ;
{
    int           indx, i ;
    double        lmin[3], lmax[3], mid[3] ;

    /* find the mid point and the appropriate child cell */

    indx = 0 ;
    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    for ( i=0 ; i<3 ; i++ ) {
        if ( coord[i] > mid[i] ) {
            indx |= (1<<i) ;  lmin[i] = mid[i] ;  lmax[i] = max[i] ;
        }
        else {
            lmin[i] = min[i] ;  lmax[i] = mid[i] ;
        }
    }

    /* check to see if the cell has children.  If so, decend the tree.
       If not, set it's flag */

    if ( tree->child[indx] ) {
        Msh3DOctMark( tree->child[indx], lmax, lmin, coord ) ;
    }
    else {
        tree->flags |= (1<<indx) ;
    }
}
#endif

/* -------------------------------------------------------------------
** Msh3DOctSize - given the coordinates of a point, this routine finds
**                the terminal cell that contains the point, and returns
**                the size of this cell.
*/

static int Msh3DOctSize(
    Msh3DOctTree  tree,
    double        max[3],
    double        min[3],
    double        face_coord[3],
    double        tree_coord[3],
    int           tree_level,
    double        *size )
{
    int           indx, i ;
    double        lmin[3], lmax[3], mid[3] ;

    /* find the mid point and the appropriate child cell */

    indx = 0 ;
    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    for ( i=0 ; i<3 ; i++ ) {
        if ( face_coord[i] > mid[i] ) {
            indx |= (1<<i) ;  lmin[i] = mid[i] ;  lmax[i] = max[i] ;
        }
        else {
            lmin[i] = min[i] ;  lmax[i] = mid[i] ;
        }
    }

    /* check to see if the cell has children.  If so, decend the tree.
       If not, set it's flag */

    if ( tree->child[indx] ) {
        return ( Msh3DOctSize( tree->child[indx], lmax, lmin, face_coord, tree_coord, tree_level+1, size ) );
    }
    else {
        /* tree center and size */

        if ( !(tree->flags & (1<<indx)) ) {
         *size = lmax[0] - lmin[0] ;
         tree_coord[0] = (lmin[0]+lmax[0])/2.0 ;
         tree_coord[1] = (lmin[1]+lmax[1])/2.0 ;
         tree_coord[2] = (lmin[2]+lmax[2])/2.0 ;
        }
        else {
         *size = 0.0 ;
         tree_coord[0] = face_coord[0] ;
         tree_coord[1] = face_coord[1] ;
         tree_coord[2] = face_coord[2] ;
        }
        return tree_level ;
    }
}

/* -------------------------------------------------------------------
** Msh3DOctClass - this routine classifies the tree. The cells are of
**                 the three types: cells that are in the boundary of
**                 the body, cells that are outside of the body   and
**                 cells inside of the body. This routine also  marks
**                 those ones that are in the boundary or outside  of
**                 the body.
*/

static void Msh3DOctClass(
    Msh3DOctTree  tree,
    int           num_org_nodes,
    int           num_org_faces,
    double        **original_nodes,
    int           **original_faces,
    double        max[3],
    double        min[3] )
{
    int           indx, i, j, k, mark ;
    double        lmin[3], lmax[3], mid[3], cmid[3] ;
    double        cen[3], vec[3], angle, dot ;
    double        coords[3][3], cross[3], pit[3], r[3], s[3] ;
    double        area, length, height, norm, d_pit, d_cen ;

    /* find the mid point of the cell */

    for ( i=0; i<3; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    /* visit all the child cells */

    for ( indx=0; indx<8; indx++ ) {
     if ( tree->child[indx] ) {
         for ( i=0 ; i<3 ; i++ ) {
	     if ( indx & (1<<i) ) {
    	         lmin[i] = mid[i] ;  lmax[i] = max[i] ;
	     }
	     else {
	         lmin[i] = min[i] ;  lmax[i] = mid[i] ;
	     }
	}
	Msh3DOctClass( tree->child[indx], num_org_nodes, num_org_faces,
	      original_nodes, original_faces, lmax, lmin ) ;
      }
      else {

        /* initiate mark for this terminal cell */

        mark = 0 ;

	/* find the limits of the terminal cell */

	for( i=0; i<3; i++ ) {
           if ( indx & (1<<i) ) {
	       lmin[i] = mid[i] ; lmax[i] = max[i] ;
           }
	   else {
	       lmin[i] = min[i] ; lmax[i] = mid[i] ;
           }
        }

	/* find the centroid of the terminal cell */

        cen[0] = (lmin[0]+lmax[0])/2.0 ;
        cen[1] = (lmin[1]+lmax[1])/2.0 ;
        cen[2] = (lmin[2]+lmax[2])/2.0 ;

        /* mark the appropiate bit in the flag word of the cell that is
	   outside of the body */

        if( !mark ) {

         /* loop trough all boundary faces to increase the contribution to
 	    the solid angle to this centroid */

         angle = 0.0 ;
         for( i = 0; i < num_org_faces; i++ )
         {
          angle += Msh3DOctClassSolidAngle( original_nodes,original_faces[i][0],
           original_faces[i][1], original_faces[i][2], cen ) ;
         }

	 /* mark the cell if the centroid is outside of the body */

         if( !((angle>2*PI)||(angle<-2*PI)) )
          mark = 1 ;

         /* if this cell was marked by the faces, mark the appropiate bit
            in the flag word */

         if( mark )
          tree->flags |= (1<<indx) ;
        }

        /* mark the appropiate bit in the flag word of the cell that is
	   in the boundary of the body */

        if( !mark ) {

         /* loop trough all boundary faces to see if the volum of the
            tetrahedron formed by the face and the centroid of the cell is
            lower than the volum of the tetrahedron formed by the the face
            and its optimal point, what represent that the centroid will
            make a very poor tetrahedron if it's choosen by this face */

         for( i = 0; i < num_org_faces; i++ )
         {
          /* find the coords of the face */

          for( j = 0; j < 3; j++ )
           for( k = 0; k < 3; k++ )
            coords[j][k] = original_nodes[original_faces[i][j]][k] ;

          /* find the center, the normal and the area of the face */

          for( j = 0; j < 3; j++ ) {
           cmid[j] = 0.0 ;
           for ( k=0 ; k<3 ; k++ ) cmid[j] += coords[k][j] ;
           cmid[j] /= 3.0 ;
          }
          for ( j=0 ; j<3 ; j++ ) {
           r[j] = coords[1][j] - coords[0][j] ;
           s[j] = coords[2][j] - coords[0][j] ;
          }
          cross[0] = r[1] * s[2] - s[1] * r[2] ;
          cross[1] = r[2] * s[0] - s[2] * r[0] ;
          cross[2] = r[0] * s[1] - s[0] * r[1] ;
          area = sqrt( cross[0] * cross[0] +
                       cross[1] * cross[1] +
                       cross[2] * cross[2] ) / 2.0 ;

          /* find the optimal point (divided by a factor) of the face */

          length = sqrt( (4*area) / sqrt(3.0) ) ;
          height = (length * sqrt(13.0)) / 4.0 ;
          norm = sqrt( cross[0] * cross[0] +
                       cross[1] * cross[1] +
                       cross[2] * cross[2] ) ;
          cross[0] /= norm ;
          cross[1] /= norm ;
          cross[2] /= norm ;
          pit[0] = cmid[0] + (height/NEW_NODES) * cross[0] ;
          pit[1] = cmid[1] + (height/NEW_NODES) * cross[1] ;
          pit[2] = cmid[2] + (height/NEW_NODES) * cross[2] ;

          /* the test shouldn't be done is the cen isn't in the side of
             the face */

          vec[0] = cen[0] - cmid[0] ;
          vec[1] = cen[1] - cmid[1] ;
          vec[2] = cen[2] - cmid[2] ;
          dot = vec[0]*cross[0] + vec[1]*cross[1] + vec[2]*cross[2] ;
          if( dot <= 0.0 ) continue ;

          /* find the volums of the two tetrahedrons */

          Msh3DOctClassVolum( coords, pit, cen, &d_pit, &d_cen );

          /* mark the cell if the volum of the tetrahedron formed by the face
             and the centroid of the cell is less than the volum of the
             tetrahedron formed by the face and its optimal point */

          if( fabs(d_cen) < fabs(d_pit) )
           mark = 1 ;

          /* if this cell was marked by this face, mark the appropiate bit
             in the flag word and break the loop because it's not necessary
             test against other faces anymore */

          if( mark ) {
           tree->flags |= (1<<indx) ;
           break ;
          }
         }
        }
    }
 }
}

static void Msh3DOctClassFull(
    Msh3DOctTree  tree,
    int           num_org_nodes,
    int           num_org_faces,
    double        **original_nodes,
    int           **original_faces,
    double        max[3],
    double        min[3] )
{
    int           indx, i, j, k, found = 0 ;
    double        lmin[3], lmax[3], mid[3] ;
    double        fmin[3], fmax[3], cen[3], angle ;

    /* find the mid point of the cell */

    for ( i=0; i<3; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    /* verify if the cell has child */

    for ( indx=0; indx<8; indx++ )
     if ( !tree->child[indx] )
      found++ ;

    /* visit all the child cells */

    if ( !found ) {
     for ( indx=0; indx<8; indx++ ) {
      if ( tree->child[indx] ) {
          for ( i=0 ; i<3 ; i++ ) {
	      if ( indx & (1<<i) ) {
    	          lmin[i] = mid[i] ;  lmax[i] = max[i] ;
	      }
	      else {
	          lmin[i] = min[i] ;  lmax[i] = mid[i] ;
	      }
	 }
	 Msh3DOctClassFull( tree->child[indx], num_org_nodes, num_org_faces,
	       original_nodes, original_faces, lmax, lmin ) ;
       }
      }
    }

    /* if this child is terminal, then classify the cell and mark those
       ones that are in the boundary or outside of the body */

    else {

     /* find the limits of the terminal cell */

     for( i=0; i<3; i++ ) {
       lmax[i] = max[i] ;
       lmin[i] = min[i] ;
     }

     /* find the centroid of the terminal cell */

     cen[0] = (lmin[0]+lmax[0])/2.0 ;
     cen[1] = (lmin[1]+lmax[1])/2.0 ;
     cen[2] = (lmin[2]+lmax[2])/2.0 ;

     /* mark the appropiate bit in the flag word of the cell that is
        in the boundary of the body */

     for( i = 0; i < num_org_faces; i++ )
     {
      /* find the cube related to the face */

      for( j=0; j<3; j++ ) {
       fmin[j] = INFPOS ;
       fmax[j] = INFNEG ;
      }
      for( j=0; j<3; j++ ) {
       for( k=0; k<3; k++ ) {
        fmax[k] = (fmax[k] > original_nodes[original_faces[i][j]][k]) ?
                   fmax[k] : original_nodes[original_faces[i][j]][k] ;
        fmin[k] = (fmin[k] < original_nodes[original_faces[i][j]][k]) ?
                   fmin[k] : original_nodes[original_faces[i][j]][k] ;
       }
      }

      /* mark the cell if it is in the boundary. This is done verifying
         if the cube related to the face intersects the cell */

      if( !( ( ((lmin[0]+(lmax[0]-lmin[0]))<=fmin[0]) ||
               ((lmin[1]+(lmax[1]-lmin[1]))<=fmin[1]) ||
	       ((lmin[2]+(lmax[2]-lmin[2]))<=fmin[2])  ) ||
	     ( ((fmin[0]+(fmax[0]-fmin[0]))<=lmin[0]) ||
	       ((fmin[1]+(fmax[1]-fmin[1]))<=lmin[1]) ||
	       ((fmin[2]+(fmax[2]-fmin[2]))<=lmin[2])  )  )  )
       tree->flags = 1 ;
     }

     /* mark the appropiate bit in the flag word of the cell that is
        outside of the body */

     angle = 0.0 ;
     for( i = 0; i < num_org_faces; i++ )
     {
      /* loop trough all boundary faces to increase the contribution to
         the solid angle to the centroid of the cell */

      angle += Msh3DOctClassSolidAngle( original_nodes,original_faces[i][0],
       original_faces[i][1], original_faces[i][2], cen ) ;
     }

     /* mark the cell if the centroid is outside of the body */

     if( !((angle>2*PI)||(angle<-2*PI)) )
      tree->flags = 1 ;
    }
}

static double Msh3DOctClassSolidAngle(
    double             **original_nodes,
    int                v0,
    int		       v1,
    int                v2,
    double             cen[3] )
{
    double              *c0, *c1, *c2 ;
    double              area = 0.0, mod, dplane, dot ;
    double              r[3], s[3], r_x_s[3], a[3], r1[3], b[3] ;

    c0 = original_nodes[v0] ;
    c1 = original_nodes[v1] ;
    c2 = original_nodes[v2] ;

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

    a[0] = c2[0]-c0[0] ; a[1] = c2[1]-c0[1] ; a[2] = c2[2]-c0[2] ;
    r1[0] = cen[0]-c0[0] ; r1[1] = cen[1]-c0[1] ; r1[2] = cen[2]-c0[2] ;
    b[0] = c1[0]-c0[0] ; b[1] = c1[1]-c0[1] ; b[2] = c1[2]-c0[2] ;
    area += Msh3DOctClassSolidEdge( cen, v0, v1, a, b, r1, r_x_s ) ;

    a[0] = -1.0*b[0] ; a[1] = -1.0*b[1] ; a[2] = -1.0*b[2] ;
    r1[0] = cen[0]-c1[0] ; r1[1] = cen[1]-c1[1] ; r1[2] = cen[2]-c1[2] ;
    b[0] = c2[0]-c1[0] ; b[1] = c2[1]-c1[1] ; b[2] = c2[2]-c1[2] ;
    area += Msh3DOctClassSolidEdge( cen, v1, v2, a, b, r1, r_x_s ) ;

    a[0] = -1.0*b[0] ; a[1] = -1.0*b[1] ; a[2] = -1.0*b[2] ;
    r1[0] = cen[0]-c2[0] ; r1[1] = cen[1]-c2[1] ; r1[2] = cen[2]-c2[2] ;
    b[0] = c0[0]-c2[0] ; b[1] = c0[1]-c2[1] ; b[2] = c0[2]-c2[2] ;
    area += Msh3DOctClassSolidEdge( cen, v2, v0, a, b, r1, r_x_s ) ;

    area -= PI ;
    dot = ( r_x_s[0] * r1[0] + r_x_s[1] * r1[1] + r_x_s[2] * r1[2] ) ;
    return (dot>0.0) ? (-area) : (area) ;
}

static double Msh3DOctClassSolidEdge(
    double             cen[3],
    int                vi,
    int                vj,
    double             a[3],
    double             b[3],
    double             r1[3],
    double             r_x_s[3] )
{
    double             n1[3], n2[3], l1, l2, s, ang, ba[3] ;

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

static double Msh3DOctClassVolumFace(
    double             coord_v[4][3],
    int                base_v,
    int                v[3] )
{
    int                i ;
    double             vol, area[3], cross[3] ;

    /* compute the area for the face */

    area[0] = area[1] = area[2] = 0.0 ;
    for( i = 0; i < 3; i++ ) {

     cross[0] = coord_v[v[i]][1] * coord_v[v[(i+1)%3]][2] -
                coord_v[v[i]][2] * coord_v[v[(i+1)%3]][1] ;
     cross[1] = coord_v[v[i]][2] * coord_v[v[(i+1)%3]][0] -
                coord_v[v[i]][0] * coord_v[v[(i+1)%3]][2] ;
     cross[2] = coord_v[v[i]][0] * coord_v[v[(i+1)%3]][1] -
                coord_v[v[i]][1] * coord_v[v[(i+1)%3]][0] ;

     area[0] += ((1/6.) * cross[0]) ;
     area[1] += ((1/6.) * cross[1]) ;
     area[2] += ((1/6.) * cross[2]) ;
    }

    /* compute the volume contribution for the face */

    vol = area[0] * coord_v[base_v][0] +
          area[1] * coord_v[base_v][1] +
          area[2] * coord_v[base_v][2] ;

    return vol ;
}

static void Msh3DOctClassVolum(
    double             coords[3][3],
    double             pit[3],
    double             cen[3],
    double             *d_pit,
    double             *d_cen )
{
    int                i, j, verts[3] ;
    double             coord_verts[4][3] ;

    /* initialize volum for pit and cen */

    (*d_pit) = (*d_cen) = 0.0 ;

    /* fill vertexs */

    for( i = 0; i < 3; i++ )
     for( j = 0; j < 3; j++ )
      coord_verts[i][j] = coords[i][j] ;

    /* set pit as point out the plane face in vertexs */

    for( j = 0; j < 3; j++ )
     coord_verts[3][j] = pit[j] ;

    /* compute volum for face plus pit */

    verts[0] = 0 ;
    verts[1] = 1 ;
    verts[2] = 2 ;
    (*d_pit) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 1 ;
    verts[1] = 0 ;
    verts[2] = 3 ;
    (*d_pit) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 2 ;
    verts[1] = 1 ;
    verts[2] = 3 ;
    (*d_pit) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 0 ;
    verts[1] = 2 ;
    verts[2] = 3 ;
    (*d_pit) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;

    /* set cen as point out the plane face in vertexs */

    for( j = 0; j < 3; j++ )
     coord_verts[3][j] = cen[j] ;

    /* compute volum for face plus pit */

    verts[0] = 0 ;
    verts[1] = 1 ;
    verts[2] = 2 ;
    (*d_cen) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 1 ;
    verts[1] = 0 ;
    verts[2] = 3 ;
    (*d_cen) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 2 ;
    verts[1] = 1 ;
    verts[2] = 3 ;
    (*d_cen) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
    verts[0] = 0 ;
    verts[1] = 2 ;
    verts[2] = 3 ;
    (*d_cen) += Msh3DOctClassVolumFace( coord_verts, verts[0], verts ) ;
}

/* -------------------------------------------------------------------
** Msh3DOctGenNodes - generate nodes at the center of all unmarked cells.
*/

#define MSH3D_NODE_QUANTUM 100
static int nodes_alloced = 0 ;

static void Msh3DOctGenNodes(
    Msh3DOctTree  tree,
    double        max[3],
    double        min[3],
    int           *num_nodes,
    double        **nodes )
{
    int           indx, i ;
    double        lmin[3], lmax[3], mid[3] ;

    /* find the mid point of the cell */

    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    /* visit all the child cells */

    for ( indx=0 ; indx<8 ; indx++ ) {
        if ( tree->child[indx] ) {
            for ( i=0 ; i<3 ; i++ ) {
                if ( indx & (1<<i) ) {
                    lmin[i] = mid[i] ;  lmax[i] = max[i] ;
                }
                else {
                    lmin[i] = min[i] ;  lmax[i] = mid[i] ;
                }
            }
            Msh3DOctGenNodes( tree->child[indx], lmax, lmin,
                              num_nodes, nodes ) ;
        }

        /* if this child is terminal, then check for a mark
           and generate a node */

        else {
            if ( !(tree->flags & (1<<indx)) ) {

                /* find the limits of the terminal cell */

                for ( i=0 ; i<3 ; i++ ) {
                    if ( indx & (1<<i) ) {
                        lmin[i] = mid[i] ;  lmax[i] = max[i] ;
                    }
                    else {
                        lmin[i] = min[i] ;  lmax[i] = mid[i] ;
                    }
                }

                /* create the node if the cell isn't out of the boundary */

                if ( !( (lmin[0] >= bmax[0]) ||
                        (lmin[1] >= bmax[1]) ||
                        (lmin[2] >= bmax[2]) ||
                        (lmax[0] <= bmin[0]) ||
                        (lmax[1] <= bmin[1]) ||
                        (lmax[2] <= bmin[2]) ) ) {

                /* allocate space if we need it */

                if ( *num_nodes >= nodes_alloced ) {
                    if ( !nodes_alloced ) {
                        nodes_alloced = MSH3D_NODE_QUANTUM ;
                        *nodes = (double *)Msh3DMalloc(
                            nodes_alloced * 3 * sizeof(double)) ;
                    }
                    else {
                        nodes_alloced += MSH3D_NODE_QUANTUM ;
                        *nodes = (double *)Msh3DRealloc( *nodes,
                            nodes_alloced * 3 * sizeof(double)) ;
                    }
                }

                /* create the node */


                for ( i=0 ; i<3 ; i++ )
                   (*nodes)[(*num_nodes)*3+i] = (lmin[i]+lmax[i])/2.0 ;
                *num_nodes += 1 ;
               }
            }
        }
    }
}

static void Msh3DOctGenNodesFull(
    Msh3DOctTree  tree,
    double        max[3],
    double        min[3],
    int           *num_nodes,
    double        **nodes )
{
    int           indx, i, found = 0 ;
    double        lmin[3], lmax[3], mid[3] ;

    /* find the mid point of the cell */

    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    /* verify if cell has child */

    for ( indx=0; indx<8; indx++ )
     if ( !tree->child[indx] )
      found++ ;

    /* visit all the child cells */

    if ( !found ) {
     for ( indx=0 ; indx<8 ; indx++ ) {
        if ( tree->child[indx] ) {
            for ( i=0 ; i<3 ; i++ ) {
                if ( indx & (1<<i) ) {
                    lmin[i] = mid[i] ;  lmax[i] = max[i] ;
                }
                else {
                    lmin[i] = min[i] ;  lmax[i] = mid[i] ;
                }
            }
            Msh3DOctGenNodesFull( tree->child[indx], lmax, lmin,
                              num_nodes, nodes ) ;
        }
      }
    }

    /* if this child is terminal, then check for a mark
       and generate a node */

    else {
      if ( !tree->marks /* !tree->flags */ ) {

      /* allocate space if we need it */

      if ( *num_nodes >= nodes_alloced ) {
        if ( !nodes_alloced ) {
          nodes_alloced = MSH3D_NODE_QUANTUM ;
          *nodes = (double *)Msh3DMalloc(
                 nodes_alloced * 3 * sizeof(double)) ;
         }
         else {
          nodes_alloced += MSH3D_NODE_QUANTUM ;
          *nodes = (double *)Msh3DRealloc( *nodes,
                 nodes_alloced * 3 * sizeof(double)) ;
         }
       }

       /* create the node */

       for ( i=0 ; i<3 ; i++ ) {
        lmax[i] = max[i] ;
        lmin[i] = min[i] ;

        (*nodes)[(*num_nodes)*3+i] = (lmin[i]+lmax[i])/2.0 ;
       }
       *num_nodes += 1 ;
      }
    }
}

/* -------------------------------------------------------------------
** Msh3DOctDBound - display the octreee boundary.
*/

static void Msh3DOctDBound( double min[3], double max[3] )
{
#if OCT_DISP
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], min[2], max[0], min[1], min[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], max[2], max[0], min[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], max[1], max[2], max[0], max[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], max[1], min[2], max[0], max[1], min[2] ) ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], min[2], min[0], min[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], min[1], min[2], max[0], min[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], max[1], min[2], max[0], max[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], max[1], min[2], min[0], max[1], max[2] ) ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], min[2], min[0], max[1], min[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], min[1], min[2], max[0], max[1], min[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], min[1], max[2], max[0], max[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], max[2], min[0], max[1], max[2] ) ;
#else
    return ;
#endif
}

/* -------------------------------------------------------------------
** Msh3DOctDTree - display the octreee innards.
*/

static void Msh3DOctDTree( Msh3DOctTree tree, double min[3], double max[3] )
{
    int     i, j ;
    double  lmin[3], lmax[3], mid[3] ;

#if OCT_DISP
    if ( !tree ) return ;
    for ( i=0 ; i<3 ; i++ ) mid[i] = (max[i] + min[i]) / 2.0 ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], mid[1], mid[2], max[0], mid[1], mid[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], min[1], mid[2], mid[0], max[1], mid[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], mid[1], min[2], mid[0], mid[1], max[2] ) ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], mid[1], min[2], min[0], mid[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], mid[2], min[0], max[1], mid[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], mid[1], min[2], max[0], mid[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            max[0], min[1], mid[2], max[0], max[1], mid[2] ) ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], min[1], mid[2], max[0], min[1], mid[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], min[1], min[2], mid[0], min[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], max[1], mid[2], max[0], max[1], mid[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], max[1], min[2], mid[0], max[1], max[2] ) ;

    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], mid[1], min[2], max[0], mid[1], min[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], min[1], min[2], mid[0], max[1], min[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            min[0], mid[1], max[2], max[0], mid[1], max[2] ) ;
    printf( "define line = {{ %f %f %f } { %f %f %f }}\n",
            mid[0], min[1], max[2], mid[0], max[1], max[2] ) ;

    for ( j=0 ; j<8 ; j++ ) {
        if ( tree->child[j] ) {
            for ( i=0 ; i<3 ; i++ ) {
                if ( j & (1<<i) ) {
                    lmin[i] = mid[i] ;  lmax[i] = max[i] ;
                }
                else {
                    lmin[i] = min[i] ;  lmax[i] = mid[i] ;
                }
            }
            Msh3DOctDTree( tree->child[j], lmin, lmax ) ;
        }
    }
#else
    for (i = 0; i < 3; i++) {
     lmin[i] = 0 ;
     lmax[i] = 0 ;
    }
    for (j = 0; j < 3; j++) {
     mid[j] = 0 ;
    }
    return ;
#endif
}

/* -------------------------------------------------------------------
** Msh3DOctDNode - display the internal nodes.
*/

static void Msh3DOctDNodes( int *g_num, double **g_nodes )
{
    int     i ;

#if OCT_DISP
    int     num   = *g_num ;
    double *nodes = *g_nodes ;
    for ( i=0 ; i<num ; i++ ) {
        printf( "define point = { %f %f %f }\n",
            nodes[i*3], nodes[i*3+1], nodes[i*3+2] ) ;
    }
#else
    i = 0 ;
    return ;
#endif
}

/* -------------------------------------------------------------------
** Msh3DOctAlloc - these routines manage the allocation and freeing
**                 oct tree entries.
*/

#define MSH3D_OCT_TREE_QUANTUM 100
static Msh3DOctTree oct_free = 0 ;
static Msh3DOctTree oct_block_ptr = 0 ;

static Msh3DOctTree Msh3DOctAlloc(void)
{
    Msh3DOctTree  new_block, alloced ;
    int           i ;

    /* if the free pointer is null we need to allocate a new block
       of boundary nodes */

    if ( !oct_free ) {
        new_block = (Msh3DOctTree)Msh3DMalloc(
           MSH3D_OCT_TREE_QUANTUM * sizeof(Msh3DOctTreeRec) ) ;
        new_block[0].child[0] = oct_block_ptr ;
        oct_block_ptr = new_block ;
        for ( i=1 ; i<(MSH3D_OCT_TREE_QUANTUM-1) ; i++ ) {
            new_block[i].child[0] = &(new_block[i+1]) ;
        }
        new_block[MSH3D_OCT_TREE_QUANTUM-1].child[0] = 0 ;
        oct_free = &(new_block[1]) ;
    }

    /* return the top thing on the free list */

    alloced = oct_free ;
    oct_free = oct_free->child[0] ;

    for ( i=0 ; i<8 ; i++ ) alloced->child[i] = 0 ;
    alloced->parent = 0 ;
    alloced->flags = 0 ;
    return( alloced ) ;
}

static void Msh3DOctFree( Msh3DOctTree oct )
{
    /* Put this face back on the free list */

#if OCT_FREE
    if (oct) oct->child[0] = oct_free ;
    oct_free = oct ;
#else
    return ;
#endif
}

static void Msh3DOctFreeAll(void)
{
    Msh3DOctTree  cur, next ;

    /* free all blocks allocated to store face information */

#if OCT_FREE
    if ( oct_block_ptr ) cur = oct_block_ptr ;
    else return ;

    while ( cur->child[0] ) {
        next = cur->child[0] ;
        Msh3DFree( cur ) ;
        cur = next ;
    }
    Msh3DFree( cur ) ;

    oct_free = 0 ;
    oct_block_ptr = 0 ;
#else
    cur = next = 0 ;
    return ;
#endif
}

//added by Markos
Msh3DOctTree Msh3DTree( )
{
	return root;
}

Box3d Msh3DTreeBox( )
{
	return box;
}
//end added by Markos
