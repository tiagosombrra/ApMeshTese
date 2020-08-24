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
**
** ----------------------------------------------------------------
*/

#ifndef JmeshPointTree_h
#define JmeshPointTree_h

#define POINT_TREE_CACHE_BLOCK_SIZE 1000

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

typedef double PointCoord[3] ;

typedef struct PointNode_ {
  struct PointNode_ *llink ;
  struct PointNode_ *rlink ;
  int         c_data ;
  int         active ;
  PointCoord  pt ;
  PointCoord  gmin ;
  PointCoord  gmax ;
} PointNode ;

typedef struct PointNodeCache_ {
  struct PointNodeCache_ *next ;
  PointNode entries[POINT_TREE_CACHE_BLOCK_SIZE] ;
} PointNodeCache ;

/*
** -----------------------------------------------------------------
** Public Functions:
*/

void InitPointTree (int num, PointCoord *pts, int *client_data) ;
void CleanPointTree (void) ;
void AddToPointTree (PointCoord pt, int client_data) ;
void RemoveFromPointTree (PointCoord pt, int client_data) ;
int *QueryPointTree (PointCoord imin, PointCoord imax, int *size) ;
void QueryCleanPointTree (void) ;
void PrintPointTree (void) ;

#endif
