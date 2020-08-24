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
**
** ----------------------------------------------------------------
*/

#ifndef JmeshRangeTree_h
#define JmeshRangeTree_h

#define RANGE_TREE_CACHE_BLOCK_SIZE 1000

#define true	1
#define false	0

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/*
** -----------------------------------------------------------------
** Public Types:
*/

typedef double RangeCoord[3] ;

typedef struct RangeNode_ {
  struct RangeNode_ *llink ;
  struct RangeNode_ *rlink ;
  int c_data ;
  int active ;
  RangeCoord  min ;
  RangeCoord  max ;
  RangeCoord  mid ;
  RangeCoord  gmin ;
  RangeCoord  gmax ;
} RangeNode ;

typedef struct RangeNodeCache_ {
  struct RangeNodeCache_ *next ;
  RangeNode entries[RANGE_TREE_CACHE_BLOCK_SIZE] ;
} RangeNodeCache ;

/*
** -----------------------------------------------------------------
** Public Functions:
*/

void InitRangeTree (int num, RangeCoord *min, RangeCoord *max, int *client_data) ;
void CleanRangeTree (void) ;
void AddToRangeTree (RangeCoord min, RangeCoord max, int client_data) ;
void RemoveFromRangeTree (RangeCoord min, RangeCoord max, int client_data) ;
int *QueryRangeTree (RangeCoord imin, RangeCoord imax, int *size) ;
void QueryCleanRangeTree (void) ;

#endif
