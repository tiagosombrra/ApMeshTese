/*
** ---------------------------------------------------------------
**
** JmeshPointTree - Point tree module.
**
** ---------------------------------------------------------------
**
** Description:
**  This module implements functions that creates an alternating
**  decision tree (ADT) for points to speed up searches.
**
** Copyright:
**  (c) 2003 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Feb-17-2003	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PostDoc in Cornell.
**   It was based on Wash's implementation in C++ for ADTs.
**  Modified:	Feb-19-2003	Joaquim Bento Cavalcante Neto
**   Included function QueryCleanPointTree to clean the memory
**   associated to a query, to be called after the query use.
**   Also changed functions QueryPointTree and QueryPointTreeRec
**   and added variable alloca_size to deal with the memory
**   allocated for query in a more appropriate and correct way.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ----------------------------------------------------------------
*/

#include "PJmesh/JmeshPointTree.h"

#include <stdio.h>

#define MAX(a,b)                (((a)>=(b))?(a):(b))
#define MIN(a,b)                (((a)<=(b))?(a):(b))

#define RESULT_BLOCK_SIZE 	5000

/* --------------------------------------------------------------
** Private variables:
*/

static int             *result ;
static int              result_size ;
static int              alloca_size ;
static PointNode       *root ;
static int              FreeList ;
static PointNodeCache *CacheList ;

/* --------------------------------------------------------------
** Private functions prototypes:
*/

void       AddToPointTreeRec(PointNode  *tree, PointCoord  pt,
		             int client_data, int level) ;
void       BuildInitialPointTree(PointNode **tree, int num_entry,
		                 int level, PointCoord pts[],
			         int client_data[]) ;
void       ChildPointBounds(PointNode *tree) ;
int        PointMedian(int num_entry, int comp, PointCoord *mid) ;
void       QueryPointTreeRec(PointNode *tree, PointCoord qmin,
		             PointCoord qmax, int level) ;
int        RemoveFromPointTreeRec(PointNode *tree, PointCoord pt,
	 	                  int client_data, int level) ;
int        PointSplitList(int num_entry, int comp,
		          PointCoord pts[], int client_data[]) ;
void       PointSwap(int i1, int i2, PointCoord pts[], int client_data[]) ;
PointNode *NewPointNode(PointCoord pt, int c_data) ;
void       PrintPointTreeRec(PointNode *tree, int level) ;

/* --------------------------------------------------------------
** Public functions:
*/

/* --------------------------------------------------------------
**
**    InitPointTree - Initialize the tree.
**
**      InitPointTree(
**              int               num,
**              PointCoord       *min,
**              PointCoord       *max,
**              int              *client_data)
**
**        num         - (in)  number of input points
**        min         - (in)  array of point minimum coordinates
**        max         - (in)  array of point maximum coordinates
**        client_data - (in)  array of client data stored with the point
**
**      Description: Initiation method, creates a new balanced ADT.
**
**
** -- */

void InitPointTree(int         num,
                   PointCoord *pts,
                   int        *client_data)
{
    int         i ;
    int         *tc_data ;
    PointCoord  *tpts ;

    FreeList = -1;
    CacheList = 0 ;
    root = 0 ;

    /* build the initial point tree */

    tc_data = calloc (num, sizeof(int)) ;
    tpts = (PointCoord *)calloc (num, sizeof(PointCoord)) ;

    for (i=0 ; i<num ; ++i) {
        tpts[i][0] = pts[i][0] ;
        tpts[i][1] = pts[i][1] ;
        tpts[i][2] = pts[i][2] ;
        tc_data[i] = client_data[i] ;
    }

    BuildInitialPointTree(&root,num,0,tpts,tc_data) ;

    free (tpts) ;
    free (tc_data) ;
}


/* -------------------------------------------------------------
**
**    CleanPointTree - Clean the point tree.
**
**      Description: Cleaner for the ADT.
**
**
** -- */

void CleanPointTree (void)
{
    PointNodeCache *ptr = CacheList ;
    while (ptr != 0) {
        PointNodeCache *tmp = ptr->next ;
	free (ptr) ;
        ptr = tmp ;
    }
}


/* -------------------------------------------------------------
**
**    AddToPointTree - add a new node to the point tree.
**
**      void AddToPointTree(
**                     PointCoord  pt,
**                     int         client_data)
**
**        pt          - (in)  point coordinate
**        client_data - (in)  user data
**
**      Description: Add a new point to the tree.
**
**
** -- */

void AddToPointTree(PointCoord  pt,
                    int         client_data)
{
    AddToPointTreeRec(root,pt,client_data,0) ;
}

/* -------------------------------------------------------------
**
**    RemoveFromPointTree - remove a point from the tree.
**
**      void RemoveFromTree(
**              PointCoord  pt,
**              int         client_data)
**
**        pt          - (in)  point coordinates
**        client_data - (in)  client data
**
**      Description: This method removes a point from the tree.
**          The delete is done if the client data matches client data
**          found in the tree.
**
**
** -- */

void RemoveFromPointTree(PointCoord pt,
                         int client_data)
{
    int found ;
    found = RemoveFromPointTreeRec(root,pt,client_data,0) ;
    /* assert(found) ; */
}

/* -------------------------------------------------------------
**
**    QueryPointTree - find all tree nodes for the range.
**
**
**      int *QueryPointTree(
**              PointCoord imin
**              PointCoord imax
**              int       *size)
**
**        imin  - (in)  minimum coordinate
**        imax  - (in)  maximum coordinate
**        size  - (out) size of the result array
**
**      Description: This method returns the client data for all tree
**          nodes found in the given range.
**
**      Return Value: array containing the client data for found nodes.
**
**
** -- */

int *QueryPointTree(PointCoord  imin,
                    PointCoord  imax,
                    int        *size)
{
    alloca_size = RESULT_BLOCK_SIZE ;
    result = (int *) calloc (alloca_size, sizeof (int)) ;
    result_size = 0 ;
    QueryPointTreeRec(root,imin,imax,0) ;
    *size = result_size ;
    return(result) ;
}

/* -------------------------------------------------------------
**
**    QueryCleanPointTree - clean query.
**
**
**      void QueryCleanPointTree (void)
**
**      Description: This method cleans the result of query range.
**
** -- */

void QueryCleanPointTree (void)
{
 if (result != 0) free (result) ;
 result = 0 ;
 alloca_size = 0 ;
 result_size = 0 ;
}

/* -------------------------------------------------------------
**
**    PrintPointTree - print the tree.
**
**      void PrintPointTree(void)
**
**      Description: This is a method to print the whole tree.
**
** -- */

void PrintPointTree (void)
{
   PrintPointTreeRec(root,0) ;
}

/* --------------------------------------------------------------
** Private functions:
*/

/* -------------------------------------------------------------
**
**    AddToPointTreeRec - recursive add to tree method.
**
**      void AddToPointTreeRec(
**              PointNode       *tree,
**              PointCoord       pt,
**              int              client_data,
**              int              level)
**
**        tree        - (in)  local root node in tree
**        pt          - (in)  point ooordinate
**        client_data - (in)  client data
**        level       - (in)  current tree level
**
**      Description: This is an add to tree method that recursively
**          decends the tree and adds a new node in the appropriate
**          location.
**
**
** -- */

void AddToPointTreeRec(PointNode   *tree,
                       PointCoord   pt,
                       int client_data,
		       int level)
{
    int i ;
    int key = level % 3 ;
    int is_active ;

    if (tree->c_data == client_data) return ;
    if ((tree->llink != 0) && (tree->llink->c_data == client_data)) return ;
    if ((tree->rlink != 0) && (tree->rlink->c_data == client_data)) return ;

    if ((tree->llink != 0) && (tree->rlink != 0)) {
        if (pt[key] < tree->pt[key]) {
            AddToPointTreeRec(tree->llink,pt,client_data,level+1) ;
        } else {
            AddToPointTreeRec(tree->rlink,pt,client_data,level+1) ;
        }

        /* update the global limits on the way up */

        ChildPointBounds(tree) ;
        for (i=0 ; i<3 ; i++) {
            if (tree->gmin[i] > tree->pt[i]) tree->gmin[i] = tree->pt[i] ;
            if (tree->gmax[i] < tree->pt[i]) tree->gmax[i] = tree->pt[i] ;
        }
        return ;
    }

    /* otherwise put the data here */

    if (tree->llink == 0) {

        /* first deal with the case of no left child.  Here there
        ** are two cases.  Either the new node goes to the left
        ** child or else it replaces the current node that goes to
        ** the left. */

        if (pt[key] < tree->pt[key]) {
            tree->llink = NewPointNode(pt,client_data) ;
        } else {
            is_active = tree->active ;
            tree->llink = NewPointNode(tree->pt,tree->c_data) ;
            tree->pt[0] = pt[0] ;
            tree->pt[1] = pt[1] ;
            tree->pt[2] = pt[2] ;
            tree->c_data = client_data ;
            tree->active = true ;
            tree->llink->active = is_active ;
        }

    } else {

        /* Else the left child exists, if the new data is bigger
        ** than the current node it goes to the right */

        if (pt[key] >= tree->pt[key]) {

            tree->rlink = NewPointNode(pt,client_data) ;

        } else {

            /* the data at the current tree node must go to the
            ** right child */

            is_active = tree->active ;
            tree->rlink = NewPointNode(tree->pt,tree->c_data) ;
            tree->rlink->active = is_active ;

            /* check to see if the new data goes to the node
            ** or if we need move the left child to the node
            ** and put the new data on the left. */

            if (tree->llink->pt[key] < pt[key]) {

                tree->pt[0] = pt[0] ;
                tree->pt[1] = pt[1] ;
                tree->pt[2] = pt[2] ;
                tree->c_data = client_data ;
                tree->active = true ;

            } else {

                tree->pt[0] = tree->llink->pt[0] ;
                tree->pt[1] = tree->llink->pt[1] ;
                tree->pt[2] = tree->llink->pt[2] ;
                tree->c_data = tree->llink->c_data ;
                tree->active = tree->llink->active ;

                tree->llink->pt[0] = pt[0] ;
                tree->llink->pt[1] = pt[1] ;
                tree->llink->pt[2] = pt[2] ;
                tree->llink->c_data = client_data ;
                tree->llink->active = true ;

            }
        }
    }

    ChildPointBounds(tree) ;
    for (i=0 ; i<3 ; ++i) {
        if (tree->gmin[i] > tree->pt[i]) tree->gmin[i] = tree->pt[i] ;
        if (tree->gmax[i] < tree->pt[i]) tree->gmax[i] = tree->pt[i] ;
    }
}


/* -------------------------------------------------------------
**
**    RemoveFromPointTreeRec - recursive remove from tree.
**
**      int  RemoveFromPointTreeRec(
**              PointNode      *tree,
**              PointCoord      pt,
**              int             client_data,
**              int             level)
**
**        tree        - (in)  local tree root
**        pt          - (in)  pt coordinate
**        client_data - (in)  client data
**        level       - (in)  tree level
**
**      Description: This is an remove from tree method that
**          recursively decends the tree and remove a node in
**          the appropriate location.
**
**      Return Value: true if the node was found.
**
**
** -- */

int  RemoveFromPointTreeRec(PointNode   *tree,
                            PointCoord   pt,
                            int          client_data,
                            int          level)
{
    int found ;

    if (client_data == tree->c_data) {
        tree->active = false ;
        ChildPointBounds(tree) ;
        return(true) ;
    }

    /* check the left subtree */

    found = false ;

    if ((tree->llink != 0) &&
        ((pt[0]>=tree->llink->gmin[0]&&
	  pt[1]>=tree->llink->gmin[1]&&
	  pt[2]>=tree->llink->gmin[2]) &&
	 (pt[0]<=tree->llink->gmax[0]&&
	  pt[1]<=tree->llink->gmax[1]&&
	  pt[2]<=tree->llink->gmax[2]))) {
        found = RemoveFromPointTreeRec(tree->llink,pt,client_data,level+1) ;
    }

    /* check the right subtree */

    if (!found && (tree->rlink != 0) &&
        ((pt[0]>=tree->rlink->gmin[0]&&
	  pt[1]>=tree->rlink->gmin[1]&&
	  pt[2]>=tree->rlink->gmin[2]) &&
	 (pt[0]<=tree->rlink->gmax[0]&&
	  pt[1]<=tree->rlink->gmax[1]&&
	  pt[2]<=tree->rlink->gmax[2]))) {
        found = RemoveFromPointTreeRec(tree->rlink,pt,client_data,level+1) ;
    }

    /* update the global bounds */

    if (found) {
        ChildPointBounds(tree) ;
	tree->gmin[0] = MIN(tree->gmin[0],tree->pt[0]) ;
	tree->gmin[1] = MIN(tree->gmin[1],tree->pt[1]) ;
	tree->gmin[2] = MIN(tree->gmin[2],tree->pt[2]) ;
	tree->gmax[0] = MAX(tree->gmax[0],tree->pt[0]) ;
	tree->gmax[1] = MAX(tree->gmax[1],tree->pt[1]) ;
	tree->gmax[2] = MAX(tree->gmax[2],tree->pt[2]) ;
    }

    return(found) ;
}


/* -------------------------------------------------------------
**
**    ChildPointBounds - sets a nodes global bound.
**
**      void ChildPointBounds(PointNode *tree)
**
**        tree - (i/o) tree node.
**
**      Description: This method sets the global bounds of a node so
**          that it is the max(min) of the value in this node and all
**          its children.
**
**
** -- */

void ChildPointBounds(PointNode *tree)
{
    if (tree->llink != 0) {
        if (tree->rlink == 0) {
            tree->gmin[0] = tree->llink->gmin[0] ;
            tree->gmin[1] = tree->llink->gmin[1] ;
            tree->gmin[2] = tree->llink->gmin[2] ;
            tree->gmax[0] = tree->llink->gmax[0] ;
            tree->gmax[1] = tree->llink->gmax[1] ;
            tree->gmax[2] = tree->llink->gmax[2] ;
        } else {
	    tree->gmin[0] = MIN(tree->llink->gmin[0],tree->rlink->gmin[0]) ;
	    tree->gmin[1] = MIN(tree->llink->gmin[1],tree->rlink->gmin[1]) ;
	    tree->gmin[2] = MIN(tree->llink->gmin[2],tree->rlink->gmin[2]) ;
	    tree->gmax[0] = MAX(tree->llink->gmax[0],tree->rlink->gmax[0]) ;
	    tree->gmax[1] = MAX(tree->llink->gmax[1],tree->rlink->gmax[1]) ;
	    tree->gmax[2] = MAX(tree->llink->gmax[2],tree->rlink->gmax[2]) ;
        }
    } else if (tree->rlink != 0) {
        tree->gmin[0] = tree->rlink->gmin[0] ;
        tree->gmin[1] = tree->rlink->gmin[1] ;
        tree->gmin[2] = tree->rlink->gmin[2] ;
        tree->gmax[0] = tree->rlink->gmax[0] ;
        tree->gmax[1] = tree->rlink->gmax[1] ;
        tree->gmax[2] = tree->rlink->gmax[2] ;
    }
}

/* -------------------------------------------------------------
**
**    QueryPointTreeRec - recursive range query.
**
**      void QueryPointTreeRec(
**              PointNode  *tree,
**              PointCoord  qmin,
**              PointCoord  qmax,
**              int         level)
**
**        tree   - (in)  local tree root.
**        qmin   - (in)  minimum query coordinate.
**        qmax   - (in)  maximum query coordinate.
**        level  - (in)  tree level.
**
**      Description: This is a recursive method that finds all the
**          nodes that intersect or are contained in a given range
**
**
** -- */

void QueryPointTreeRec(PointNode   *tree,
                       PointCoord   qmin,
                       PointCoord   qmax,
                       int          level)
{
    int key = level % 3 ;

    /* Check to see if the current node overlaps the query box. */

    if (tree->active) {
        if ((tree->pt[0] >= qmin[0]) &&
            (tree->pt[0] <= qmax[0]) &&
            (tree->pt[1] >= qmin[1]) &&
            (tree->pt[1] <= qmax[1]) &&
            (tree->pt[2] >= qmin[2]) &&
            (tree->pt[2] <= qmax[2])) {
            if(result_size < alloca_size) {
             result[result_size] = tree->c_data ;
            } else {
             alloca_size += RESULT_BLOCK_SIZE ;
             result = (int *) realloc (result, alloca_size * sizeof(int)) ;
	     result[result_size] = tree->c_data ;
            }
	    result_size++ ;
        }
    }

    /* check the left subtree */

    if ((tree->llink != 0) && (qmin[key] <= tree->gmax[key])) {
        QueryPointTreeRec(tree->llink,qmin,qmax,level+1) ;
    }

    /* check the right subtree */

    if ((tree->rlink != 0) && (qmax[key] >= tree->gmin[key])) {
        QueryPointTreeRec(tree->rlink,qmin,qmax,level+1) ;
    }
}

/* -------------------------------------------------------------
**
**    NewPointNode - create a new tree node.
**
**      PointNode *NewPointNode(
**                        PointCoord  pt,
**                        int         c_data)
**
**        pt     - (in)  pt coordinate value
**        c_data - (in)  client data
**
**      Description: Create and fill a new tree node.
**
**      Return Value: the new tree node.
**
**
** -- */

PointNode *NewPointNode(PointCoord  pt,
                        int         c_data)
{
    int             i ;
    PointNodeCache *ctmp ;
    PointNode      *tmp ;

    if (FreeList < 0) {
     ctmp = (PointNodeCache *)malloc(sizeof(PointNodeCache));
     ctmp->next = CacheList ;
     CacheList = ctmp ;
     FreeList = POINT_TREE_CACHE_BLOCK_SIZE-1 ;
    }
    tmp = &(CacheList->entries[FreeList]) ;
    FreeList-- ;
    for (i = 0; i < 3; i++) {
     tmp->pt[i] = pt[i] ;
     tmp->gmin[i] = pt[i] ;
     tmp->gmax[i] = pt[i] ;
    }
    tmp->c_data = c_data ;
    tmp->llink = 0 ;
    tmp->rlink = 0 ;
    tmp->active = true ;
    return(tmp) ;
}

/* -------------------------------------------------------------
**
**    BuildInitialPointTree - recursive build tree.
**
**      void BuildInitialPointTree(
**              PointNode  **tree,
**              int          num_entry,
**              int          level,
**              PointCoord   pts[],
**              int          client_data[])
**
**        tree        - (in)  pointer to the local tree root
**        num_entry   - (in)  number of new entries
**        level       - (in)  current level
**        pts         - (in)  array of pts coordinates
**        client_data - (in)  array of client data
**
**      Description: This is a recursive method used to build the
**          original tree.
**
**
** -- */

void BuildInitialPointTree(PointNode **tree,
                           int num_entry, int level,
                           PointCoord  pts[], int client_data[])
{
    int i ;
    int key = level % 3 ;

    int list_divider = PointSplitList(num_entry,key,pts,client_data) ;
    (*tree) = NewPointNode(pts[list_divider],client_data[list_divider]) ;

    if (list_divider != 0) {

        /* Add left subtree and recursively fill it. */

        BuildInitialPointTree(&((*tree)->llink),list_divider,level+1,
                         pts,client_data) ;
    }

    if (list_divider != (num_entry - 1)) {

        /* Add right subtree and recursively fill it. */

        BuildInitialPointTree(&((*tree)->rlink),(num_entry-list_divider-1),
                 level+1,&pts[list_divider+1],&client_data[list_divider+1]) ;
    }

    ChildPointBounds(*tree) ;
    for (i=0 ; i<3 ; ++i) {
        if ((*tree)->pt[i] < (*tree)->gmin[i])
            (*tree)->gmin[i] = (*tree)->pt[i] ;
        if ((*tree)->pt[i] > (*tree)->gmax[i])
            (*tree)->gmax[i] = (*tree)->pt[i] ;
    }
}


/* -------------------------------------------------------------
**
**    PointMedian - find the median value.
**
**      int PointMedian(
**              int             num_entry,
**              int             comp,
**              PointCoord     *mid)
**
**        num_entry - (in)  number of values in the list
**        comp      - (in)  index (x,y,z) to use for comparison
**        mid       - (in)  list of coordinate values
**
**      Description: This method finds the median value of a list of
**          coordinates.
**
**      Return Value: index of the median value.
**
**
** -- */

int PointMedian(int num_entry, int comp,
                PointCoord *mid)
{
    /* algorithm is from Numerical Recipies */

    int num_left, num_below, i ;
    int *index_list ;
    int num_less, num_eq, ret_val ;
    int next_slot, swap ;
    double comp_value ;

    if (num_entry == 1) return (0) ;
    index_list = (int *) calloc (num_entry, sizeof (int)) ;

    num_left = num_entry ;
    num_below = 0 ;

    for (i=0 ; i<num_entry ; i++) index_list[i] = i ;

    while (num_left > 2) {
        comp_value = (mid[index_list[0]][comp] +
                      mid[index_list[num_left/2]][comp] +
                             mid[index_list[num_left-1]][comp]) / 3.0 ;
        num_less = 0 ;
	num_eq = 0 ;
        for (i=0 ; i<num_left ; i++) {
            if (mid[index_list[i]][comp] <= comp_value) {
                if (mid[index_list[i]][comp] < comp_value)
                    num_less++ ;
                else
                    num_eq++ ;
            }
        }

        if (num_less + num_eq == 0 || num_less == num_left ||
            num_eq == num_left) {

            ret_val = index_list[0] ;
	    free (index_list) ;
            return ret_val ;

        }  else if (num_less + num_below > num_entry / 2) {

            /* Select numbers less than comp_value for further
            ** searching. */
            next_slot = 0 ;
            for (i=0 ; i<num_left ; i++) {
                if (mid[index_list[i]][comp] < comp_value) {
                    index_list[next_slot] = index_list[i] ;
                    next_slot++ ;
                }
            }
            num_left = num_less ;
            assert (next_slot == num_left) ;

        } else if (num_less + num_eq + num_below > num_entry / 2) {

            /* Select numbers less than or equal to comp_value
            ** for further searching. */
            next_slot = 0 ;
            for (i=0 ; i<num_left ; i++) {
                if (mid[index_list[i]][comp] == comp_value) {
                    index_list[next_slot] = index_list[i] ;
                    next_slot++ ;
                }
            }
            num_left = num_eq ;
            num_below += num_less ;
            assert (next_slot == num_eq) ;

        } else {

            /* Select numbers greater than comp_value for
            ** further searching. */
            next_slot = 0 ;
            for (i=0 ; i<num_left ; i++) {
                if (mid[index_list[i]][comp] > comp_value) {
                    index_list[next_slot] = index_list[i] ;
                    next_slot++ ;
                }
            }
            num_left = num_left - num_less - num_eq ;
            assert(next_slot == num_left) ;
            num_below += num_less + num_eq;

        }
    }

    if (num_left == 1) {
        ret_val = index_list[0] ;
    } else {
        if (mid[index_list[0]][comp] > mid[index_list[1]][comp]) {
            swap = index_list[0] ;
            index_list[0] = index_list[1] ;
            index_list[1] = swap ;
        }
        ret_val = (num_below + num_left > num_entry/2 + 1) ?
                   index_list[0] : index_list[1] ;
    }
    free (index_list) ;
    return (ret_val) ;
}


/* -------------------------------------------------------------
**
**    PointSwap - swap list values.
**
**      void PointSwap(
**              int         i1,
**              int         i2,
**              PointCoord  pts[],
**              int         client_data[])
**
**        i1          - (in)  first index
**        i2          - (in)  second index
**        pts         - (i/o) list of point values
**        client_data - (i/o) list of client data
**
**      Description: Used by PointSplitList to swap values in list.
**
**
** -- */

void PointSwap(int i1,int i2,PointCoord pts[],int client_data[])
{
    int        itmp ;
    PointCoord ctmp ;

    ctmp[0] = pts[i1][0] ;
    ctmp[1] = pts[i1][1] ;
    ctmp[2] = pts[i1][2] ;
    pts[i1][0] = pts[i2][0] ;
    pts[i1][1] = pts[i2][1] ;
    pts[i1][2] = pts[i2][2] ;
    pts[i2][0] = ctmp[0] ;
    pts[i2][1] = ctmp[1] ;
    pts[i2][2] = ctmp[2] ;
    itmp = client_data[i1] ;
    client_data[i1] = client_data[i2] ;
    client_data[i2] = itmp ;
}


/* -------------------------------------------------------------
**
**    PointSplitList - split a list about the median.
**
**      int PointSplitList(
**              int         num_entry,
**              int         comp,
**              PointCoord  pts[],
**              int         client_data[])
**
**        num_entry   - (in)  number of values in the list
**        comp        - (in)  component (x,y,z) to sort on
**        pts         - (i/o) list of point values
**        client_data - (i/o) list of client data
**
**      Description: This method finds the median value in a list and
**          swaps values so that values less than the median come
**          before it in the list.
**
**      Return Value: index of the median value
**
**
** -- */

int PointSplitList(int num_entry, int comp,PointCoord pts[],
		   int client_data[])
{
    int pivot ;
    int left_end, right_end ;
    int divider, balance ;
    double comp_val ;

    if (num_entry == 1) return (0) ;
    pivot = PointMedian(num_entry,comp,pts) ;
    PointSwap(pivot,0,pts,client_data) ;

    comp_val = pts[0][comp] ;
    left_end = 0 ;
    right_end = num_entry ;

    while (left_end < right_end) {
        left_end++ ;
        right_end-- ;

        while ((pts[left_end][comp] < comp_val) &&
               (left_end < right_end)) left_end++ ;

        while ((pts[right_end][comp] > comp_val) &&
               (left_end <= right_end)) right_end-- ;

        if (left_end < right_end)
            PointSwap(left_end,right_end,pts,client_data) ;
    }

    PointSwap(right_end,0,pts,client_data) ;

    divider = right_end ;
    balance = num_entry - 2*divider - 1 ;

    if (balance >= 2) {
        left_end  = divider ;
        right_end = num_entry ;
        while ((left_end < right_end) && (balance >= 2)) {
            left_end++ ;
            right_end-- ;

            while ((pts[left_end][comp] <= comp_val) &&
                   (left_end < right_end)) {
                if (pts[left_end][comp] == comp_val) {
                    divider++ ;
                    balance -= 2 ;
                }
                left_end++ ;
            }

            while ((pts[right_end][comp] != comp_val) &&
                   (left_end < right_end)) {
                right_end-- ;
            }

            if ((left_end < right_end) && (balance >= 2)) {
                PointSwap(left_end,right_end,pts,client_data) ;
                divider++ ;
                balance -= 2 ;
            }
        }

    } else if (balance <= -2) {

        /* Fix up equal keys to the left of the divider. */

        left_end  = -1 ;
        right_end = divider ;
        while ((left_end < right_end) && (balance <= -2)) {
            left_end++ ;
            right_end-- ;

            while ((pts[left_end][comp] != comp_val) &&
                   (left_end < right_end)) {
                left_end++ ;
            }

            while ((pts[right_end][comp] >= comp_val) &&
                   (left_end < right_end)) {
                if (pts[right_end][comp] == comp_val) {
                    divider-- ;
                    balance += 2 ;
                }
                right_end-- ;
            }

            if ((left_end < right_end) && (balance <= -2)) {
              PointSwap(left_end,right_end,pts,client_data) ;
              divider-- ;
              balance += 2 ;
            }
        }
    }

    return(divider) ;
}

/* -------------------------------------------------------------
**
**    PrintPointTreeRec - recursive print tree.
**
**      void PrintPointTreeRec(
**              PointNode   *tree,
**              int          level,
**
**        tree        - (in)  pointer to the local tree root
**        level       - (in)  current level
**
**      Description: This is a recursive method used to print the
**          original tree.
**
**
** -- */

void PrintPointTreeRec(PointNode *tree, int level)
{
 int i ;

    for (i=0 ; i<level ; ++i) printf("    ") ;
    if (tree->active) {
	printf("%d : %g %g %g\n",tree->c_data,
	    tree->pt[0],tree->pt[1],tree->pt[2]) ;
    } else {
        printf("%d : inactive\n",tree->c_data) ;
    }

    /* check the left subtree */

    if (tree->llink != 0) {
       PrintPointTreeRec(tree->llink,level+1) ;
    }

    /* check the right subtree */

    if (tree->rlink != 0) {
       PrintPointTreeRec(tree->rlink,level+1) ;
    }
}

