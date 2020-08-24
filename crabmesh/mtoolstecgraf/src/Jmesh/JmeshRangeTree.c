/*
** ---------------------------------------------------------------
**
** JmeshRangeTree - Range tree module.
**
** ---------------------------------------------------------------
**
** Description:
**  This module implements functions that creates an alternating
**  decision tree (ADT) for bounding boxes to speed up searches.
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
**   Included function QueryCleanRangeTree to clean the memory
**   associated to a query, to be called after the query use.
**   Also changed functions QueryRangeTree and QueryRangeTreeRec
**   and added variable alloca_size to deal with the memory
**   allocated for query in a more appropriate and correct way.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ----------------------------------------------------------------
*/

#include "Jmesh/JmeshRangeTree.h"

#include <stdio.h>

#define MAX(a,b)		(((a)>=(b))?(a):(b))
#define MIN(a,b)		(((a)<=(b))?(a):(b))

#define RESULT_BLOCK_SIZE 	5000

/* --------------------------------------------------------------
** Private variables:
*/

static int           *result ;
static int            result_size ;
static int            alloca_size ;
static RangeNode      *root ;
static int            FreeList ;
static RangeNodeCache *CacheList ;

/* --------------------------------------------------------------
** Private functions prototypes:
*/

void      AddToRangeTreeRec(RangeNode *tree, RangeCoord min,
	                    RangeCoord max,  RangeCoord mid,
			    int client_data, int level) ;
void      BuildInitialRangeTree(RangeNode **tree, int num_entry, int level,
	                        RangeCoord min[], RangeCoord max[],
			        RangeCoord mid[], int client_data[]) ;
void      ChildRangeBounds(RangeNode *tree) ;
int       RangeMedian(int num_entry, int comp, RangeCoord *mid) ;
void      QueryRangeTreeRec(RangeNode *tree, RangeCoord qmin,
		            RangeCoord qmax, int level) ;
int       RemoveFromRangeTreeRecCheap(RangeNode *tree, RangeCoord qmid,
			              int client_data, int level) ;
int       RemoveFromRangeTreeRecFull(RangeNode *tree, RangeCoord qmin,
			             RangeCoord qmax, int client_data,
				     int level) ;
int       RangeSplitList(int num_entry, int comp, RangeCoord min[],
		         RangeCoord max[], RangeCoord mid[],
			 int client_data[]) ;
void      RangeSwap(int i1, int i2, RangeCoord  min[], RangeCoord max[],
	            RangeCoord  mid[], int client_data[]) ;
RangeNode *NewRangeNode(RangeCoord min, RangeCoord max,
		        RangeCoord mid, int c_data) ;

/* --------------------------------------------------------------
** Public functions:
*/

/* --------------------------------------------------------------
**
**    InitRangeTree - Initialize the tree.
**
**      InitRangeTree(
**              int             num,
**              RangeCoord      *min,
**              RangeCoord      *max,
**              int             *client_data)
**
**        num         - (in)  number of input bounding boxes
**        min         - (in)  array of bounding box minimum coordinates
**        max         - (in)  array of bounding box maximum coordinates
**        client_data - (in)  array of client data stored with the box
**
**      Description: Initiation method, creates a new balanced ADT.
**
**
** -- */

void InitRangeTree(int num,
                   RangeCoord *min,
                   RangeCoord *max,
                   int *client_data)
{
    int         i ;
    int        *tc_data ;
    RangeCoord *tmin, *tmax, *tmid ;

    FreeList = -1;
    CacheList = 0 ;
    root = 0 ;

    /* build the initial range tree */

    tc_data = (int *) calloc (num, sizeof(int)) ;
    tmin = (RangeCoord *)calloc (num, sizeof(RangeCoord)) ;
    tmax = (RangeCoord *)calloc (num, sizeof(RangeCoord)) ;
    tmid = (RangeCoord *)calloc (num, sizeof(RangeCoord)) ;

    for (i=0 ; i<num ; ++i) {
        tmin[i][0] = min[i][0] ;
        tmin[i][1] = min[i][1] ;
        tmin[i][2] = min[i][2] ;
        tmax[i][0] = max[i][0] ;
        tmax[i][1] = max[i][1] ;
        tmax[i][2] = max[i][2] ;
	tmid[i][0] = 0.5*(min[i][0]+max[i][0]) ;
	tmid[i][1] = 0.5*(min[i][1]+max[i][1]) ;
	tmid[i][2] = 0.5*(min[i][2]+max[i][2]) ;
        tc_data[i] = client_data[i] ;
    }

    BuildInitialRangeTree(&root,num,0,tmin,tmax,tmid,tc_data) ;

    free (tmin) ;
    free (tmax) ;
    free (tmid) ;
    free (tc_data) ;
}


/* -------------------------------------------------------------
**
**    CleanRangeTree - Clean the tree.
**
**      Description: Cleaner for an ADT.
**
**
** -- */

void CleanRangeTree (void)
{
    RangeNodeCache *ptr = CacheList ;
    while (ptr != 0) {
        RangeNodeCache *tmp = ptr->next ;
	free (ptr) ;
        ptr = tmp ;
    }
}


/* -------------------------------------------------------------
**
**    AddToRangeTree - add a new node.
**
**      void AddToRangeTree(
**                     RangeCoord  min,
**                     RangeCoord  max,
**                     int         client_data)
**
**        min         - (in)  minimum coordinate
**        max         - (in)  maximum coordinate
**        client_data - (in)  user data
**
**      Description: Add a new bounding box to the tree.
**
**
** -- */

void AddToRangeTree(RangeCoord  min,
                    RangeCoord  max,
                    int client_data)
{
    RangeCoord mid ;
    mid[0] = 0.5*(min[0]+max[0]) ;
    mid[1] = 0.5*(min[1]+max[1]) ;
    mid[2] = 0.5*(min[2]+max[2]) ;
    AddToRangeTreeRec(root,min,max,mid,client_data,0) ;
}

/* -------------------------------------------------------------
**
**    RemoveFromRangeTree - remove a bounding box from the tree.
**
**      void RemoveFromRangeTree(
**              RangeCoord  min,
**              RangeCoord  max,
**              int        client_data)
**
**        min         - (in)  minimum coordinate
**        max         - (in)  maximum coordinate
**        client_data - (in)  client data
**
**      Description: This method removes a bounding box from the tree.
**          The delete is done if the client data matches client data
**          found in the tree. The max and min values are given to
**          speed the search
**
**
** -- */

void RemoveFromRangeTree(RangeCoord min,
                         RangeCoord max,
                         int client_data)
{
    int found ;
    RangeCoord mid ;
    mid[0] = 0.5 * (max[0]+min[0]) ;
    mid[1] = 0.5 * (max[1]+min[1]) ;
    mid[2] = 0.5 * (max[2]+min[2]) ;
    found = RemoveFromRangeTreeRecCheap(root,mid,client_data,0) ;
    if (found) return ;
    found = RemoveFromRangeTreeRecFull(root,min,max,client_data,0) ;
    /* assert(found) ; */
}

/* -------------------------------------------------------------
**
**    QueryRangeTree - find all tree nodes in the given range.
**
**
**      int *QueryRangeTree(
**              RangeCoord min,
**              RangeCoord max,
**              int        *size)
**
**        min   - (in)  minimum coordinate
**        max   - (in)  maximum coordinate
**        size  - (out) size of the result array
**
**      Description: This method returns the client data for all tree
**          nodes found in the given range.
**
**      Return Value: array containing the client data for found nodes.
**
**
** -- */

int *QueryRangeTree(RangeCoord  imin,
                    RangeCoord  imax,
		    int        *size)
{
    alloca_size = RESULT_BLOCK_SIZE ;
    result = (int *) calloc (alloca_size, sizeof (int)) ;
    result_size = 0 ;
    QueryRangeTreeRec(root,imin,imax,0) ;
    *size = result_size ;
    return(result) ;
}

/* -------------------------------------------------------------
**
**    QueryCleanRangeTree - clean query.
**
**
**      void QueryCleanRangeTree (void)
**
**      Description: This method cleans the result of query range.
**
** -- */

void QueryCleanRangeTree (void)
{
 if (result != 0) free (result) ;
 result = 0 ;
 alloca_size = 0 ;
 result_size = 0 ;
}

/* --------------------------------------------------------------
** Private functions:
*/

/* -------------------------------------------------------------
**
**    AddToRangeTreeRec - recursive add to tree method.
**
**      void AddToRangeTreeRec(
**              RangeNode       *tree,
**              RangeCoord      min,
**              RangeCoord      max,
**              RangeCoord      mid,
**              int             client_data,
**              int             level)
**
**        tree        - (in)  local root node in tree
**        min         - (in)  maximum value
**        max         - (in)  minimum value
**        mid         - (in)  mid value
**        client_data - (in)  client data
**        level       - (in)  current tree level
**
**      Description: This is an add to tree method that recursively
**          decends the tree and adds a new node in the appropriate
**          location.
**
**
** -- */

void AddToRangeTreeRec(RangeNode  *tree,
                       RangeCoord  min,
                       RangeCoord  max,
                       RangeCoord  mid,
                       int         client_data,
		       int         level)
{
    int i, is_active ;
    int key = level % 3 ;

    if ((tree->llink != 0) && (tree->rlink != 0)) {
        if (mid[key] < tree->mid[key]) {
            AddToRangeTreeRec(tree->llink,min,max,mid,client_data,level+1) ;
        } else {
            AddToRangeTreeRec(tree->rlink,min,max,mid,client_data,level+1) ;
        }

        /* update the global limits on the way up */

        ChildRangeBounds(tree) ;
        for (i=0 ; i<3 ; ++i) {
            if (tree->min[i] < tree->gmin[i]) tree->gmin[i] = tree->min[i] ;
            if (tree->max[i] > tree->gmax[i]) tree->gmax[i] = tree->max[i] ;
        }
        return ;
    }

    /* otherwise put the data here */

    if (tree->llink == 0) {

        /* first deal with the case of no left child.  Here there
        ** are two cases.  Either the new node goes to the left
        ** child or else it replaces the current node that goes to
        ** the left. */

        if (mid[key] < tree->mid[key]) {
            tree->llink = NewRangeNode(min,max,mid,client_data) ;
        } else {
            is_active = tree->active ;
            tree->llink = NewRangeNode(tree->min,tree->max,tree->mid,
                                       tree->c_data) ;
            tree->min[0] = min[0] ;
            tree->min[1] = min[1] ;
            tree->min[2] = min[2] ;
            tree->max[0] = max[0] ;
            tree->max[1] = max[1] ;
            tree->max[2] = max[2] ;
            tree->mid[0] = mid[0] ;
            tree->mid[1] = mid[1] ;
            tree->mid[2] = mid[2] ;
            tree->c_data = client_data ;
            tree->active = true ;
            tree->llink->active = is_active ;
        }

    } else {

        /* Else the left child exists, if the new data is bigger
        ** than the current node it goes to the right */

        if (mid[key] >= tree->mid[key]) {

            tree->rlink = NewRangeNode(min,max,mid,client_data) ;

        } else {

            /* the data at the current tree node must go to the
            ** right child */

            is_active = tree->active ;
            tree->rlink = NewRangeNode(tree->min,tree->max,tree->mid,
                                       tree->c_data) ;
            tree->rlink->active = is_active ;

            /* check to see if the new data goes to the node
            ** or if we need move the left child to the node
            ** and put the new data on the left. */

            /* int next_key = (level+1) % 3 ;
            ** if (tree->llink->mid[next_key] < mid[next_key]) */

            if (tree->llink->mid[key] < mid[key]) {

                tree->min[0] = min[0] ;
                tree->min[1] = min[1] ;
                tree->min[2] = min[2] ;
                tree->max[0] = max[0] ;
                tree->max[1] = max[1] ;
                tree->max[2] = max[2] ;
                tree->mid[0] = mid[0] ;
                tree->mid[1] = mid[1] ;
                tree->mid[2] = mid[2] ;
                tree->c_data = client_data ;
                tree->active = true ;

            } else {

                tree->min[0] = tree->llink->min[0] ;
                tree->min[1] = tree->llink->min[1] ;
                tree->min[2] = tree->llink->min[2] ;
                tree->max[0] = tree->llink->max[0] ;
                tree->max[1] = tree->llink->max[1] ;
                tree->max[2] = tree->llink->max[2] ;
                tree->mid[0] = tree->llink->mid[0] ;
                tree->mid[1] = tree->llink->mid[1] ;
                tree->mid[2] = tree->llink->mid[2] ;
                tree->c_data = tree->llink->c_data ;
                tree->active = tree->llink->active ;

                tree->llink->min[0] = min[0] ;
                tree->llink->min[1] = min[1] ;
                tree->llink->min[2] = min[2] ;
                tree->llink->max[0] = max[0] ;
                tree->llink->max[1] = max[1] ;
                tree->llink->max[2] = max[2] ;
                tree->llink->mid[0] = mid[0] ;
                tree->llink->mid[1] = mid[1] ;
                tree->llink->mid[2] = mid[2] ;
                tree->llink->c_data = client_data ;
                tree->llink->active = true ;

            }
        }
    }

    ChildRangeBounds(tree) ;
    for (i=0 ; i<3 ; ++i) {
        if (tree->min[i] < tree->gmin[i]) tree->gmin[i] = tree->min[i] ;
        if (tree->max[i] > tree->gmax[i]) tree->gmax[i] = tree->max[i] ;
    }
}


/* -------------------------------------------------------------
**
**    RemoveFromRangeTreeRec - recursive remove from tree.
**
**      int  RemoveFromRangeTreeRec(
**              RangeNode      *tree,
**              RangeCoord      qmid,
**              int             client_data,
**              int             level)
**
**        tree        - (in)  local tree root
**        qmid        - (in)  mid coordinate
**        client_data - (in)  client data
**        level       - (in)  tree level
**
**      Description: This is a recursive method that locates a node
**          and removes it from the tree in a cheap way.
**
**      Return Value: tree if the node was found.
**
**
** -- */

int  RemoveFromRangeTreeRecCheap(RangeNode  *tree,
                                 RangeCoord  qmid,
                                 int         client_data,
                                 int         level)
{
    int  found = false ;
    int key = level % 3 ;

    if (client_data == tree->c_data) {
        tree->active = false ;
        ChildRangeBounds(tree) ;
        return(true) ;
    }

    /* check the left subtree */

    if ((tree->llink != 0) && (qmid[key] <= tree->mid[key])) {
     found = RemoveFromRangeTreeRecCheap(tree->llink,qmid,client_data,level+1) ;
    }

    /* check the right subtree */

    if (!found && (tree->rlink != 0) && (qmid[key] >= tree->mid[key])) {
     found = RemoveFromRangeTreeRecCheap(tree->rlink,qmid,client_data,level+1) ;
    }

    /* update the global bounds */

    if (found) {
        ChildRangeBounds(tree) ;
	tree->gmin[0] = MIN(tree->gmin[0],tree->min[0]) ;
	tree->gmin[1] = MIN(tree->gmin[1],tree->min[1]) ;
	tree->gmin[2] = MIN(tree->gmin[2],tree->min[2]) ;
	tree->gmax[0] = MAX(tree->gmax[0],tree->max[0]) ;
	tree->gmax[1] = MAX(tree->gmax[1],tree->max[1]) ;
	tree->gmax[2] = MAX(tree->gmax[2],tree->max[2]) ;
    }

    return(found) ;
}


/* -------------------------------------------------------------
**
**    RemoveFromRangeTreeRecFull - recursive remove from tree full.
**
**      int  RemoveFromRangeTreeRec(
**              RangeNode      *tree,
**              RangeCoord      qmin,
**              RangeCoord      qmax,
**              int             client_data,
**              int             level)
**
**        tree        - (in)  local tree root
**        qmin        - (in)  min coordinate
**        qmax        - (in)  max coordinate
**        client_data - (in)  client data
**        level       - (in)  tree level
**
**      Description: This is a recursive method that locates a node
**          and removes it from the tree in a full way.
**
** -- */

int  RemoveFromRangeTreeRecFull(RangeNode  *tree,
                                RangeCoord  qmin,
                                RangeCoord  qmax,
                                int         client_data,
                                int         level)
{
    int  found = false ;

    if (client_data == tree->c_data) {
        tree->active = false ;
        ChildRangeBounds(tree) ;
        return(true) ;
    }

    /* check the left subtree */

    if ((tree->llink != 0) &&
        ((qmin[0]>=tree->llink->gmin[0]&&
	  qmin[1]>=tree->llink->gmin[1]&&
	  qmin[2]>=tree->llink->gmin[2]) &&
	 (qmax[0]<=tree->llink->gmax[0]&&
	  qmax[1]<=tree->llink->gmax[1]&&
	  qmax[2]<=tree->llink->gmax[2]))) {
     found = RemoveFromRangeTreeRecFull(tree->llink,qmin,qmax,client_data,level+1) ;
    }

    /* check the right subtree */

    if (!found && (tree->rlink != 0) &&
        ((qmin[0]>=tree->rlink->gmin[0]&&
	  qmin[1]>=tree->rlink->gmin[1]&&
	  qmin[2]>=tree->rlink->gmin[2]) &&
	 (qmax[0]<=tree->rlink->gmax[0]&&
	  qmax[1]<=tree->rlink->gmax[1]&&
	  qmax[2]<=tree->rlink->gmax[2]))) {
     found = RemoveFromRangeTreeRecFull(tree->rlink,qmin,qmax,client_data,level+1) ;
    }

    /* update the global bounds */

    if (found) {
        ChildRangeBounds(tree) ;
	tree->gmin[0] = MIN(tree->gmin[0],tree->min[0]) ;
	tree->gmin[1] = MIN(tree->gmin[1],tree->min[1]) ;
	tree->gmin[2] = MIN(tree->gmin[2],tree->min[2]) ;
	tree->gmax[0] = MAX(tree->gmax[0],tree->max[0]) ;
	tree->gmax[1] = MAX(tree->gmax[1],tree->max[1]) ;
	tree->gmax[2] = MAX(tree->gmax[2],tree->max[2]) ;
    }

    return(found) ;
}


/* -------------------------------------------------------------
**
**    ChildRangeBounds - sets a nodes global bound.
**
**      void ChildRangeBounds(RangeNode *tree)
**
**        tree - (i/o) tree node.
**
**      Description: This method sets the global bounds of a node so
**          that it is the max(min) of the value in this node and all
**          its children.
**
**
** -- */

void ChildRangeBounds(RangeNode *tree)
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
**    QueryRangeTreeRec - recursive range query.
**
**      void QueryRangeTreeRec(
**              RangeNode  *tree,
**              RangeCoord  qmin,
**              RangeCoord  qmax,
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

void QueryRangeTreeRec(RangeNode  *tree,
                       RangeCoord  qmin,
                       RangeCoord  qmax,
                       int         level)
{
    int key = level % 3 ;

    /* Check to see if the current node overlaps the query box. */

    if (tree->active) {
        if ((qmax[0] >= tree->min[0]) &&
            (qmin[0] <= tree->max[0]) &&
            (qmax[1] >= tree->min[1]) &&
            (qmin[1] <= tree->max[1]) &&
            (qmax[2] >= tree->min[2]) &&
            (qmin[2] <= tree->max[2])) {
            if (result_size < alloca_size) {
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

    if ((tree->llink != 0) && ((qmin[key] <= tree->gmax[key]) ||
        (tree->mid[key] == tree->llink->mid[key]))) {
        QueryRangeTreeRec(tree->llink,qmin,qmax,level+1) ;
    }

    /* check the right subtree */

    if ((tree->rlink != 0) && (qmax[key] >= tree->gmin[key])) {
        QueryRangeTreeRec(tree->rlink,qmin,qmax,level+1) ;
    }
}

/* -------------------------------------------------------------
**
**    NewRangeNode - create a new tree node.
**
**      RangeNode *NewRangeNode(
**                        RangeCoord  min,
**                        RangeCoord  max,
**                        RangeCoord  mid,
**                        int         c_data)
**
**        min    - (in)  minimum coordinate value
**        max    - (in)  maximum coordinate value
**        mid    - (in)  mid coordinate value
**        c_data - (in)  client data
**
**      Description: Create and fill a new tree node.
**
**      Return Value: the new tree node.
**
**
** -- */

RangeNode *NewRangeNode(RangeCoord min,
                        RangeCoord max,
                        RangeCoord mid,
                        int        c_data)
{
    int             i ;
    RangeNodeCache *ctmp ;
    RangeNode      *tmp ;

    if (FreeList < 0) {
        ctmp = (RangeNodeCache *) malloc (sizeof(RangeNodeCache)) ;
        ctmp->next = CacheList ;
        CacheList = ctmp ;
        FreeList = RANGE_TREE_CACHE_BLOCK_SIZE-1 ;
    }
    tmp = &(CacheList->entries[FreeList]) ;
    FreeList-- ;
    for (i = 0; i < 3; i++) {
     tmp->min[i] = min[i] ;
     tmp->max[i] = max[i] ;
     tmp->mid[i] = mid[i] ;
     tmp->gmin[i] = min[i] ;
     tmp->gmax[i] = max[i] ;
    }
    tmp->c_data = c_data ;
    tmp->llink = 0 ;
    tmp->rlink = 0 ;
    tmp->active = true ;
    return(tmp) ;
}

/* -------------------------------------------------------------
**
**    BuildInitialRangeTree - recursive build tree.
**
**      void BuildInitialRangeTree(
**              RangeNode  **tree,
**              int          num_entry,
**              int          level,
**              RangeCoord   min[],
**              RangeCoord   max[],
**              RangeCoord   mid[],
**              int          client_data[])
**
**        tree        - (in)  pointer to the local tree root
**        num_entry   - (in)  number of new entries
**        level       - (in)  current level
**        min         - (in)  array of minimum coordinates
**        max         - (in)  array of maximum coordinates
**        mid         - (in)  array of mid coordinates
**        client_data - (in)  array of client data
**
**      Description: This is a recursive method used to build the
**          original tree.
**
**
** -- */

void BuildInitialRangeTree(RangeNode **tree,
                           int         num_entry,
			   int         level,
                           RangeCoord  min[],
			   RangeCoord  max[],
                           RangeCoord  mid[],
			   int         client_data[])
{
    int i ;
    int key = level % 3 ;

    int list_divider = RangeSplitList(num_entry,key,min,max,mid,
                                      client_data) ;
    (*tree) = NewRangeNode(min[list_divider],max[list_divider],
                           mid[list_divider],client_data[list_divider]) ;

    if (list_divider != 0) {

        /* Add left subtree and recursively fill it. */

        BuildInitialRangeTree(&((*tree)->llink),list_divider,level+1,
                              min,max,mid,client_data) ;
    }

    if (list_divider != (num_entry - 1)) {

        /* Add right subtree and recursively fill it. */

        BuildInitialRangeTree(&((*tree)->rlink),(num_entry-list_divider-1),
                         level+1,&min[list_divider+1],&max[list_divider+1],
                         &mid[list_divider+1],&client_data[list_divider+1]) ;
    }

    ChildRangeBounds(*tree) ;
    for (i=0 ; i<3 ; ++i) {
        if ((*tree)->min[i] < (*tree)->gmin[i])
            (*tree)->gmin[i] = (*tree)->min[i] ;
        if ((*tree)->max[i] > (*tree)->gmax[i])
            (*tree)->gmax[i] = (*tree)->max[i] ;
    }
}


/* -------------------------------------------------------------
**
**    RangeMedian - find the median value.
**
**      int RangeMedian(
**              int             num_entry,
**              int             comp,
**              RangeCoord     *mid)
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

int RangeMedian(int num_entry, int comp, RangeCoord *mid)
{
    /* algorithm is from Numerical Recipies */

    int num_left, num_below, i ;
    int *index_list, swap ;
    int num_less, num_eq, ret_val, next_slot ;
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
**    RangeSwap - swap list values.
**
**      void RangeSwap(
**              int         i1,
**              int         i2,
**              RangeCoord  min[],
**              RangeCoord  max[],
**              RangeCoord  mid[],
**              int         client_data[])
**
**        i1          - (in)  first index
**        i2          - (in)  second index
**        min         - (i/o) list of minimum values
**        max         - (i/o) list of maximum values
**        mid         - (i/o) list of mid values
**        client_data - (i/o) list of client data
**
**      Description: Used by SplitList to swap values in list.
**
**
** -- */

void RangeSwap(int i1, int i2, RangeCoord  min[], RangeCoord max[],
	       RangeCoord  mid[], int client_data[])
{
    int        itmp ;
    RangeCoord ctmp ;

    ctmp[0] = min[i1][0] ;
    ctmp[1] = min[i1][1] ;
    ctmp[2] = min[i1][2] ;
    min[i1][0] = min[i2][0] ;
    min[i1][1] = min[i2][1] ;
    min[i1][2] = min[i2][2] ;
    min[i2][0] = ctmp[0] ;
    min[i2][1] = ctmp[1] ;
    min[i2][2] = ctmp[2] ;
    ctmp[0] = max[i1][0] ;
    ctmp[1] = max[i1][1] ;
    ctmp[2] = max[i1][2] ;
    max[i1][0] = max[i2][0] ;
    max[i1][1] = max[i2][1] ;
    max[i1][2] = max[i2][2] ;
    max[i2][0] = ctmp[0] ;
    max[i2][1] = ctmp[1] ;
    max[i2][2] = ctmp[2] ;
    ctmp[0] = mid[i1][0] ;
    ctmp[1] = mid[i1][1] ;
    ctmp[2] = mid[i1][2] ;
    mid[i1][0] = mid[i2][0] ;
    mid[i1][1] = mid[i2][1] ;
    mid[i1][2] = mid[i2][2] ;
    mid[i2][0] = ctmp[0] ;
    mid[i2][1] = ctmp[1] ;
    mid[i2][2] = ctmp[2] ;
    itmp = client_data[i1] ;
    client_data[i1] = client_data[i2] ;
    client_data[i2] = itmp ;
}


/* -------------------------------------------------------------
**
**    RangeSplitList - split a list about the median.
**
**      int RangeSplitList(
**              int         num_entry,
**              int         comp,
**              RangeCoord  min[],
**              RangeCoord  max[],
**              RangeCoord  mid[],
**              int         client_data[])
**
**        num_entry   - (in)  number of values in the list
**        comp        - (in)  component (x,y,z) to sort on
**        min         - (i/o) list of minimum values
**        max         - (i/o) list of maximum values
**        mid         - (i/o) list of mid values
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

int RangeSplitList(int num_entry, int comp,
                   RangeCoord  min[], RangeCoord  max[],
                   RangeCoord  mid[], int client_data[])
{
    int pivot ;
    int left_end, right_end ;
    int divider, balance ;
    double comp_val ;

    if (num_entry == 1) return (0) ;
    pivot = RangeMedian(num_entry,comp,mid) ;
    RangeSwap(pivot,0,min,max,mid,client_data) ;

    comp_val = mid[0][comp] ;
    left_end = 0 ;
    right_end = num_entry ;

    while (left_end < right_end) {
        left_end++ ;
        right_end-- ;

        while ((mid[left_end][comp] < comp_val) &&
               (left_end < right_end)) left_end++ ;

        while ((mid[right_end][comp] > comp_val) &&
               (left_end <= right_end)) right_end-- ;

        if (left_end < right_end)
            RangeSwap(left_end,right_end,min,max,mid,client_data) ;
    }

    RangeSwap(right_end,0,min,max,mid,client_data) ;

    divider = right_end ;
    balance = num_entry - 2*divider - 1 ;

    if (balance >= 2) {
        left_end  = divider ;
        right_end = num_entry ;
        while ((left_end < right_end) && (balance >= 2)) {
            left_end++ ;
            right_end-- ;

            while ((mid[left_end][comp] <= comp_val) &&
                   (left_end < right_end)) {
                if (mid[left_end][comp] == comp_val) {
                    divider++ ;
                    balance -= 2 ;
                }
                left_end++ ;
            }

            while ((mid[right_end][comp] != comp_val) &&
                   (left_end < right_end)) {
                right_end-- ;
            }

            if ((left_end < right_end) && (balance >= 2)) {
                RangeSwap(left_end,right_end,min,max,mid,client_data) ;
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

            while ((mid[left_end][comp] != comp_val) &&
                   (left_end < right_end)) {
                left_end++ ;
            }

            while ((mid[right_end][comp] >= comp_val) &&
                   (left_end < right_end)) {
                if (mid[right_end][comp] == comp_val) {
                    divider-- ;
                    balance += 2 ;
                }
                right_end-- ;
            }

            if ((left_end < right_end) && (balance <= -2)) {
              RangeSwap(left_end,right_end,min,max,mid,client_data) ;
              divider-- ;
              balance += 2 ;
            }
        }
    }

    /* balance = num_entry - 2*divider - 1 ; */

    return(divider) ;
}
