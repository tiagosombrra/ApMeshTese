/*
** ---------------------------------------------------------------------
**
** JmeshElemExt3D.c - Element extraction module.
**
** ---------------------------------------------------------------------
**
** Description:
**  This module contains operators for decomposing simple manifolds
**  through a set of element extractors for faces, vertices and edges.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**   It was based on Tilio's initial version of the extractors.
**  Modified:	Oct-25-2001	Joaquim Bento Cavalcante Neto
**   This procedure, while still being kept in the code for the sake
**   of documentation, it is not being used anymore because it does
**   not guarantee improvement on the algorithm for mesh generation.
**  Modified:	Oct-23-2001	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ---------------------------------------------------------------------
**
*/

#include "PJmesh/JmeshMainDef3D.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
** ---------------------------------------------------------------------
** Definitions for use of algorithms:
*/

/*
** ---------------------------------------------------------------------
** Private definitions:
*/

#define NEW_NODES                5
#define TOL_FACTOR               0.0001  /* changed by tol_factor */
#define SPH_FACTOR               10.0
#define ABOUT_ZERO(val,tol)      ((val<tol)&&(val>(-tol)))
#define OUT_FACE(val,tol)        ((val>(1.0+tol))||(val<(0.0-tol)))
#define SIGN(dot)                (((dot)>0.0)?(1):(-1))

/*
** ---------------------------------------------------------------------
** Static variables:
*/

static double tol ;

/*
** ---------------------------------------------------------------------
** Private functions prototypes:
*/

static int           VConvex( int, Msh3DAdjFace, Msh3DBdryNodeList ) ;
static int           EConvex( int, int, Msh3DBdryFace, Msh3DBdryFace,
		                        Msh3DBdryNodeList ) ;
static int	     VValid( int, Msh3DAdjFace, Msh3DBdryNodeList ) ;
static int           EValid( int, int, Msh3DBdryFace, Msh3DBdryFace,
		                       Msh3DBdryNodeList ) ;
static int           FValid( int, Msh3DBdryFace, Msh3DBdryNodeList, int ) ;
static int           CheckEdgeValidity( Msh3DBdryNodeList, int, int,
		                                           int, int ) ;
static int           CheckFaceValidity( Msh3DBdryNodeList, Msh3DBdryFace,
		                        int, int ) ;
static int           CheckCase1( Msh3DBdryFace, Msh3DBdryNodeList, int, int ) ;
static int           CheckCase2( Msh3DBdryFace, Msh3DBdryNodeList, int, int ) ;
static int           CheckCase3( Msh3DBdryFace, Msh3DBdryFace,
		                 Msh3DBdryNodeList, int, int ) ;
static int           InsNewNode( Msh3DBdryFace, Msh3DBdryNodeList,
		                 int *, int, int * ) ;
static Msh3DBdryFace InsNewNodeFindFace( Msh3DBdryFace, Msh3DBdryNodeList,
		                         int, int ) ;
static void          InsNewNodeFindNode( Msh3DBdryFace, int, double [3] ) ;

/*
** ---------------------------------------------------------------------
** Extract - extract operators for element extraction.
*/

int ExtractVert (
  int                nnodes,
  Msh3DBdryNodeList  node_list,
  int                *v1,
  int                *v2,
  int                *v3 )
{
  int            n, i, j, tri = 0;
  int            vi, vj;
  Msh3DAdjFace   adj ;
  Msh3DBdryFace  cur ;

  for ( i=0 ; i<nnodes; i++ ) {
    if ( node_list[i].active_flag == 2 )
     continue;
    if ( node_list[i].active_flag ) {
      for (n=0,adj=node_list[i].faces; adj; n++, adj=adj->next);
      if ( n == 3 && VConvex (i, node_list[i].faces,node_list) &&
           VValid (i, node_list[i].faces,node_list) ) {
        tri = 1;
        break;
      }
    }
  }
  if (tri) {
    adj=node_list[i].faces;
    for ( j=0 ; j<3; j++ ) {
      cur = adj->face;
      vi = cur->verts[j];
      vj = cur->verts[(j+1)%3];
      if (vi != i && vj != i ) {
        *v1 = vi;
        *v2 = vj;
      }
    }
    adj=adj->next;
    for ( j=0 ; j<3; j++ ) {
      cur = adj->face;
      vi = cur->verts[j];
      if (vi != i && vi != *v1 && vi != *v2 )
        *v3 = vi;
    }
    return i;
  }

  return -1;
}

int ExtractEdge (
  Msh3DBdryNodeList  node_list,
  Msh3DBdryFace      *f1,
  Msh3DBdryFace      *f2,
  int                *v1,
  int                *v2 )
{
  int            i, j, found;
  int            vi, vj, *verts;
  Msh3DAdjFace   adj ;
  Msh3DBdryFace  cur;

  cur = bdry_stack ;
  while (cur) {

    for ( i=0 ; i<3; i++ ) {
      vi = cur->verts[i];
      vj = cur->verts[(i+1)%3];
      *f1 = cur;
      if ( vj > vi ) {
        found = 0;
        for (adj=node_list[vi].faces; adj; adj=adj->next) {
          verts = adj->face->verts;
          for ( j=0 ; j<3; j++ ) {
            if ( verts[j] == vj &&  verts[(j+1)%3] == vi ) {
              *f2 = adj->face;
              found = 1;
              break;
            }
          }
          if (found) {
            if (EConvex(vi,vj,*f1,*f2,node_list) &&
                EValid (vi,vj,*f1,*f2,node_list) ) {
             *v1 = vi;
             *v2 = vj;
             return 1;
            }
            break;
          }
        }
      }
    }

    cur = cur->next;
  }
  return 0;
}

int ExtractFace (
  Msh3DBdryNodeList node_list,
  Msh3DBdryFace     *f1,
  int               *v1,
  int               *num_nodes )
{
  int     k ;
  int     alloc = 1 ;
  Msh3DBdryFace cur ;

  cur = bdry_stack ;
  while (cur) {

   /* fprintf (stdout,"New face popped\n"); */
   for ( k = 1; k <= NEW_NODES; k++ ) {
    /* fprintf (stdout,"Trying insertion of new node\n"); */
    if (!InsNewNode (cur, node_list, num_nodes, k, &alloc ))
     continue ;
    if ( FValid ((*num_nodes)-1, cur, node_list, k ) )
    {
     *f1 = cur ;
     *v1 = (*num_nodes) - 1 ;
     /* fprintf (stdout,"Valid  insertion of new node\n"); */
     return 1 ;
    }
   }
   /* fprintf (stdout,"Push popped face back to stack\n"); */

  cur =  cur->next ;
 }
 return 0 ;
}

/*
** ---------------------------------------------------------------------
** Convex - routines to verify if entities are convex.
*/

static int VConvex (
  int                v,
  Msh3DAdjFace       adj,
  Msh3DBdryNodeList  nl )
{
  int            i;
  int            vi = v, vj;
  Msh3DBdryFace  f1,f2;

  f1 = adj->face;
  for ( i=0 ; i<3; i++ )
    if ( f1->verts[i] == vi ) {
      vj = f1->verts[(i+1)%3];
      break;
    }

  adj = adj->next;
  while (adj) {
    for ( i=0 ; i<3; i++ )
      if ( adj->face->verts[i] == vj ) {
        f2 = adj->face;
        return EConvex(vi,vj,f1,f2,nl);
      }
    adj=adj->next;
  }

  return 0;
}

static int EConvex (
  int                vi,
  int                vj,
  Msh3DBdryFace      f1,
  Msh3DBdryFace      f2,
  Msh3DBdryNodeList  nl )
{
  int    i;
  double  n1xn2[3], e[3], n1xn2_dot_e = 0.0;
  double  tol ;

  tol = f1->area * TOL_FACTOR ;

  n1xn2[0] = f1->r_x_s[1] * f2->r_x_s[2] -
             f2->r_x_s[1] * f1->r_x_s[2] ;
  n1xn2[1] = f1->r_x_s[2] * f2->r_x_s[0] -
             f2->r_x_s[2] * f1->r_x_s[0] ;
  n1xn2[2] = f1->r_x_s[0] * f2->r_x_s[1] -
             f2->r_x_s[0] * f1->r_x_s[1] ;

  for ( i=0 ; i<3; i++ )
   e[i] = nl[vj].coord[i] - nl[vi].coord[i] ;

  for ( i=0 ; i<3; i++ )
    n1xn2_dot_e += n1xn2[i]*e[i];

  if ( ABOUT_ZERO(n1xn2_dot_e,tol) ) return 0 ;

  return (n1xn2_dot_e<0);
}

/*
** ---------------------------------------------------------------------
** Valid - routines to check geometrical validity of extract operators.
*/

static int VValid (
  int                v,
  Msh3DAdjFace       adj,
  Msh3DBdryNodeList  nl )
{
  int            i, status;
  int            vi = v, vj;
  Msh3DBdryFace  f1;

  f1 = adj->face;
  for ( i=0 ; i<3; i++ )
    if ( f1->verts[i] == vi ) {
      vj = f1->verts[(i+1)%3];
      break;
    }

  status = 1;

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
   GdbClear ();
   GdbSetTopOff (GDB_EDGE);
   GdbSetTopOff (GDB_DELFACE);
   GdbSetTopOff (GDB_NEWFACE);
   GdbSetCurVertexOn (v);
   GdbDrawBdry ();
  }
#endif

  if (!status)
    return 0;
  return status;
}

static int EValid (
  int                v1,
  int                v2,
  Msh3DBdryFace      f1,
  Msh3DBdryFace      f2,
  Msh3DBdryNodeList  nl )
{
  int               i, status, v3, v4;

  for ( i=0 ; i<3; i++ )
    if ( f1->verts[i] == v1 )
      v3 = f1->verts[(i+2)%3];

  for ( i=0 ; i<3; i++ )
    if ( f2->verts[i] == v1 )
      v4 = f2->verts[(i+1)%3];

  status = CheckEdgeValidity( nl, v3, v4, v1, v2 );

#if DSP_MESH
  if ( FDEBUG && GdbWait ) {
   GdbClear();
   GdbSetTopOff (GDB_DELFACE);
   GdbSetTopOff (GDB_NEWFACE);
   GdbSetTopOff (GDB_VERTEX);
   GdbSetTopOff (GDB_EDGE);
   GdbSetCurEdgeOn (v1,v2);
   GdbSetDelFaceOn (f1);
   GdbSetDelFaceOn (f2);
   GdbDrawBdry ( );
  }
#endif

  if (!status)
    return 0;

#if 0
  double e[3], n1xn2_dot_e = 0.0;
  Msh3DBdryFaceRec new;
  new.verts[0] = v1 ;
  new.verts[1] = v4 ;
  new.verts[2] = v3 ;

  if ( FDEBUG) {
     GdbClear ();
     GdbSetNewFaceOn (&new);
     GdbDrawBdry ();
  }

  for ( i=0 ; i<3 ; i++ ) {
    new.r[i] = nl[new.verts[1]].coord[i] -
               nl[new.verts[0]].coord[i] ;
    new.s[i] = nl[new.verts[2]].coord[i] -
               nl[new.verts[0]].coord[i] ;
  }

  new.r_x_s[0] = new.r[1]*new.s[2] - new.s[1]*new.r[2] ;
  new.r_x_s[1] = new.r[2]*new.s[0] - new.s[2]*new.r[0] ;
  new.r_x_s[2] = new.r[0]*new.s[1] - new.s[0]*new.r[1] ;
  for ( i=0 ; i<3; i++ )
    e[i] = nl[v2].coord[i] - nl[v1].coord[i];

  for ( i=0 ; i<3; i++ )
    n1xn2_dot_e += new.r_x_s[i]*e[i];
  status = (n1xn2_dot_e>0);
#endif

  return 1;
}

static int FValid (
 int	            v1,
 Msh3DBdryFace      f1,
 Msh3DBdryNodeList  nl,
 int                debug )
{
  int	status ;

  status = CheckFaceValidity( nl, f1, v1, debug );

#if DSP_MESH
  if( FDEBUG ) {
   GdbClear();
   GdbSetTopOff (GDB_DELFACE);
   GdbSetTopOff (GDB_NEWFACE);
   GdbSetTopOff (GDB_VERTEX);
   GdbSetTopOff (GDB_EDGE);
   GdbSetDelFaceOn (f1);
   GdbSetCurVertexOn (v1);
   GdbDrawBdry();
  }
#endif

  if (!status)
   return 0;
  else
   return 1;
}

static int CheckEdgeValidity(
  Msh3DBdryNodeList  node_list,
  int                vi,
  int                vj,
  int                v1,
  int                v2 )
{
  int               i, j, valid ;
  double             min[3], max[3] ;
  int               type, curvi, curvj;
  Msh3DBdryFace     current ;
  Msh3DBdryFaceRec  new[2] ;

  /* get the max and min coords for the proposed element */

  for ( i=0 ; i<3 ; i++ ) {
     min[i] = node_list[vi].coord[i];
     max[i] = node_list[vi].coord[i];
     if ( node_list[vj].coord[i] < min[i] )
        min[i] = node_list[vj].coord[i] ;
     if ( node_list[vj].coord[i] > max[i] )
        max[i] = node_list[vj].coord[i] ;
     if ( node_list[v1].coord[i] < min[i] )
        min[i] = node_list[v1].coord[i] ;
     if ( node_list[v1].coord[i] > max[i] )
        max[i] = node_list[v1].coord[i] ;
     if ( node_list[v2].coord[i] < min[i] )
        min[i] = node_list[v2].coord[i] ;
     if ( node_list[v2].coord[i] > max[i] )
        max[i] = node_list[v2].coord[i] ;
  }

  /* loop through all the faces in the current boundary and see
     if they intersect one of the two new faces */

  valid = 1 ;
  Msh3DBdryReset() ;

  while ( valid && (current = (Msh3DBdryFace) Msh3DBdryNext()) ) {

    tol = current->area * tol_factor ;

  /* first do a min/max test for the face.
     We can probably reject most faces this way */

    if ( ( min[0] > current->max[0] ) ||
         ( max[0] < current->min[0] ) ||
         ( min[1] > current->max[1] ) ||
         ( max[1] < current->min[1] ) ||
         ( min[2] > current->max[2] ) ||
         ( max[2] < current->min[2] ))
      continue ;

   /* if the bounding boxes intersect we start looking at the common
      vertices. first we look to see if the new edge has
      common vertices with the boundary triangle we are currently
      checking. */

   /* if we have a common vertex, we can have one of the two cases
      depending on the position of the other vertex of the edge:

   1) lies in a different plane than the triangle ->
      go to next check (type = 2)
   2) lies in the same plane as the triangle
      a) crossed edges -> reject  (type = 0)
      b) no crossing   -> valid   (type = 1)

   if there is no common vertex go to next check (type = 3) */

    type = CheckCase1( current, node_list, vi, vj ) ;

    if ( type == 0 ) {

#if 0
      if ( FDEBUG ) {
         GdbDspFail (vi,vi,vj);
      }
#endif
      return 0;
    }
    else if ( type == 1 ) continue;
    else if ( type == 3 ) {

   /* we have no common vertices so we do some geometrical checks.
      First check to see if the new edge intersect the current
      boundary triangle:

      intersect -> 0
      otherwise -> 1 */

      type = CheckCase2( current, node_list, vi, vj ) ;

      if ( type == 0 ) {

#if 0
        if ( FDEBUG  ) {
           GdbDspFail (vi,vi,vj);
        }
#endif
        return 0;
      }
    }

   /* now we need to see if the edges of the boundary triangle
      intersect any of the two new faces */

   /* compute the information we need for the new faces */

    new[0].verts[0] = v1 ;
    new[0].verts[1] = vj ;
    new[0].verts[2] = vi ;

    new[1].verts[0] = v2 ;
    new[1].verts[1] = vi ;
    new[1].verts[2] = vj ;

   /* compute the r and s vectors and cross products */

    for ( i=0 ; i<2 ; i++ ) {
      for ( j=0 ; j<3 ; j++ ) {
        new[i].r[j] = node_list[new[i].verts[1]].coord[j] -
                      node_list[new[i].verts[0]].coord[j] ;
        new[i].s[j] = node_list[new[i].verts[2]].coord[j] -
                      node_list[new[i].verts[0]].coord[j] ;
      }

      new[i].r_x_s[0] = new[i].r[1] * new[i].s[2] -
                        new[i].s[1] * new[i].r[2] ;
      new[i].r_x_s[1] = new[i].r[2] * new[i].s[0] -
                        new[i].s[2] * new[i].r[0] ;
      new[i].r_x_s[2] = new[i].r[0] * new[i].s[1] -
                        new[i].s[0] * new[i].r[1] ;

      new[i].rs_dot_v1 =
           new[i].r_x_s[0] * node_list[new[i].verts[0]].coord[0] +
           new[i].r_x_s[1] * node_list[new[i].verts[0]].coord[1] +
           new[i].r_x_s[2] * node_list[new[i].verts[0]].coord[2];


   /* loop for the three edges on the boundary face */

      for ( j=0 ; j<3 ; j++ ) {
        curvi = current->verts[j] ;
        curvj = current->verts[(j+1)%3] ;

   /* just as above, if the edge intersects the face, then the two
      end points of the edge must be on opposite sides of
      the face.  To check this we dot the two ends of the
      edge with the r cross s vector (normal) for the face,
      and subtract this from v1 doted with the normal.  If
      the two results have the same sign, then both are on
      the same side */

        type = CheckCase2( &new[i], node_list, curvi, curvj ) ;

        if ( type == 0 ) {

#if 0
                 if ( FDEBUG ) {
                   GdbSetNewFaceOn(&new[i]);
                   GdbDspFail (curvi,curvi,curvj);
                 }
#endif
          return 0;
        }
        else             continue;
      }
    }
  }

  return(valid) ;
}

static int  CheckFaceValidity(
    Msh3DBdryNodeList  node_list,
    Msh3DBdryFace      face,
    int                node_indx,
    int                debug )
{
    int    i, j, k, valid ;
    double  min[3], max[3] ;
    int    v1, v2, type ;
    Msh3DBdryFace  current ;
    Msh3DBdryFaceRec new ;

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

    valid = 1 ;
    Msh3DBdryReset() ;

    while ( valid && (current = (Msh3DBdryFace) Msh3DBdryNext()) ) {
        tol = current->area * tol_factor ;

        /* if current face and the base face is the same, go to
	   next face, because no geometrical test is necessary */

        if ( current == face )
	 continue ;

        /* first do a min/max test for the entire element.
           We can probably reject most faces this way */

        if (( min[0] > current->max[0] ) || ( max[0] < current->min[0] ) ||
            ( min[1] > current->max[1] ) || ( max[1] < current->min[1] ) ||
            ( min[2] > current->max[2] ) || ( max[2] < current->min[2] ))
            continue ;

        /* if the bounding boxes intersect we start looking at the common
           vertices.  first we look to see if any of the new edges has common
           vertices with the boundary triangle we are currently checking. */

        /* if we have a common vertex, we can have one of the three cases
           depending on the position of the other vertex of the edge:

           1) is also common to the triangle -> check next edge (type = 1)
           2) lies in a different plane than the triangle -> check next
	      edge (type = 1)
           3) lies in the same plane as the triangle
               a) crossed edges        -> reject  (type = 0)
               b) no crossing          -> check next edge (type = 1)
        */

        for ( j=0 ; j<3 ; j++ ) {

          v1 = node_indx ;
          v2 = face->verts[j] ;

          /* check to see if both vertices are common to the face */

          type = 0;
          for ( i = 0 ; i<3 ; i++ ) {
           if (  v1 == current->verts[i] &&
                 v2 == current->verts[(i+1)%3] )
            type = 1;
          }

          if (!type)
           type = CheckCase1( current, node_list, v1, v2 ) ;

          if ( type == 0 ) return 0;
          else if ( type == 1 ) continue;

          /* if we get here we have no common vertices so we do some geome-
             trical checks.  First check to see if the new edge intersect the
             current boundary triangle */

          type = CheckCase2( current, node_list, v1, v2 ) ;

          if ( type == 0 ) return 0;
          else             continue;
        }

        /* now we need to see if the edges of the boundary triangle intersect
           any of the three new faces */

        for ( j=0 ; j<3 ; j++ ) {  /* loop over new faces */

            /* compute the information we need for the new face */

            new.verts[0] = face->verts[j] ;
            new.verts[1] = face->verts[(j+1)%3] ;
            new.verts[2] = node_indx ;

            /* compute the r and s vectors and cross products */

            for ( i=0 ; i<3 ; i++ ) {
                new.r[i] = node_list[new.verts[1]].coord[i] -
                           node_list[new.verts[0]].coord[i] ;
                new.s[i] = node_list[new.verts[2]].coord[i] -
                           node_list[new.verts[0]].coord[i] ;
            }

            new.r_x_s[0] = new.r[1] * new.s[2] - new.s[1] * new.r[2] ;
            new.r_x_s[1] = new.r[2] * new.s[0] - new.s[2] * new.r[0] ;
            new.r_x_s[2] = new.r[0] * new.s[1] - new.s[0] * new.r[1] ;

            new.rs_dot_v1 = new.r_x_s[0] * node_list[new.verts[0]].coord[0] +
                            new.r_x_s[1] * node_list[new.verts[0]].coord[1] +
                            new.r_x_s[2] * node_list[new.verts[0]].coord[2] ;


            new.area = sqrt( new.r_x_s[0] * new.r_x_s[0] +
                             new.r_x_s[1] * new.r_x_s[1] +
                             new.r_x_s[2] * new.r_x_s[2] ) / 2.0 ;

             /* now check to see if the two faces have any vertices in common.
               If they do, then the faces can only intersect if they have only
               one common vertex, and an edge of the boudary triangle lies in
               the plane the proposed new triangle. */

            /* loop for the three edges on the boundary face */

            for ( k=0 ; k<3 ; k++ ) {

               v1 = current->verts[k] ;
               v2 = current->verts[(k+1)%3] ;

               /* check to see if both vertices are common to the face */

               type = 0;
               for ( i = 0 ; i<3 ; i++ ) {
               if (  v1 == current->verts[i] &&
                     v2 == current->verts[(i+1)%3] )
                type = 1;
               }

	       if (!type)
                type = CheckCase1( &new, node_list, v1, v2 ) ;

               if ( type == 0 ) return 0;
               else if ( type == 1 ) continue;

            /* just as above, if the edge intersects the face, then the two
               end points of the edge must be on opposite sides of
               the face.  To check this we dot the two ends of the
               edge with the r cross s vector (normal) for the face,
               and subtract this from v1 doted with the normal.  If
               the two results have the same sign, then both are on
               the same side */

               type = CheckCase2( &new, node_list, v1, v2 ) ;

               if ( type == 0 ) return 0;
               else             continue;
            }
        }

        /* test to see if new point is near from current face. If it's
	   then this point should not be considered */

        type = CheckCase3( current, face, node_list, node_indx, debug );

	if( type == 0 ) return 0;

    }

    return(valid) ;
}

static int  CheckCase1(
  Msh3DBdryFace      face,
  Msh3DBdryNodeList  node_list,
  int                v1,
  int                v2 )
{
  int    i;
  int    common, ncom;
  double  tprod1, dot ;
  double  *c1, *c2, *c3, *c0 ;
  double  r[3],s[3],t[3],r_x_t[3],s_x_t[3],r_x_s[3] ;

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

     if ( !ABOUT_ZERO(tprod1,tol) ) return 2 ;

  /* if we get here the other vertex lies in the plane of the
     triangle. we need to see if the new edge will cross on of the
     edges of the triangle. To check this we first cross one edge
     of the triangle with the proposed new edges, and then cross
     the other. These cross product vectors should be parallel.
     If they have the same sign, the edge is OK. If not we reject
     the new element */

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

     r_x_t[0] = r[1] * t[2] - t[1] * r[2] ;
     r_x_t[1] = r[2] * t[0] - t[2] * r[0] ;
     r_x_t[2] = r[0] * t[1] - t[0] * r[1] ;

     dot = s_x_t[0]*r_x_t[0] + s_x_t[1]*r_x_t[1] +
           s_x_t[2]*r_x_t[2] ;

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

  return 3;
}

static int CheckCase2(
  Msh3DBdryFace      face,
  Msh3DBdryNodeList  node_list,
  int                v1,
  int                v2 )
{
  int    i;
  double  tprod1, tprod2 ;
  double  *c1, *c2, *c3 ;
  double  u, v, w ;
  double  t[3],r_x_t[3],s_x_t[3] ;

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
     opposite sides of the face.  We then compute the
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
  u /= s_x_t[0]*face->r[0] + s_x_t[1]*face->r[1] +
         s_x_t[2]*face->r[2] ;
  if ( OUT_FACE(u,tol_factor) ) return 1 ;

  r_x_t[0] = face->r[1] * t[2] - t[1] * face->r[2] ;
  r_x_t[1] = face->r[2] * t[0] - t[2] * face->r[0] ;
  r_x_t[2] = face->r[0] * t[1] - t[0] * face->r[1] ;
  v  = r_x_t[0]*c1[0] + r_x_t[1]*c1[1] + r_x_t[2]*c1[2] ;
  v -= r_x_t[0]*c3[0] + r_x_t[1]*c3[1] + r_x_t[2]*c3[2] ;
  v /= r_x_t[0]*face->s[0] + r_x_t[1]*face->s[1] +
         r_x_t[2]*face->s[2] ;
  if ( OUT_FACE(v,tol_factor) ) return 1 ;

  w = 1.0 - u - v ;
  if ( OUT_FACE(w,tol_factor) ) return 1 ;

  /* if we get here, then the edge crosses the face
     so we should return with an invalid status */

  return  0 ;
}

static int CheckCase3(
  Msh3DBdryFace      face,
  Msh3DBdryFace      bface,
  Msh3DBdryNodeList  node_list,
  int                node_indx,
  int                debug )
{
  Msh3DBdryFace facef ;
  int           i ;
  double         *c, *c1, *c2, *c3 ;
  double         cand_vec[3], proj_vec[3], proj[3] ;
  double         r[3], s[3], r_x_s[3], area, u, v, w ;
  double         dot, edge, param, height, num, den, dist ;

  c  = node_list[node_indx].coord ;
  c1 = node_list[face->verts[0]].coord ;
  c2 = node_list[face->verts[1]].coord ;
  c3 = node_list[face->verts[2]].coord ;

  /* this test shouldn't be done against faces that were considered in the ge-
     neration of the new node */

  for( i = 0; i < 3; i++ )
  {
   facef = InsNewNodeFindFace (bface,node_list,
                               bface->verts[i],bface->verts[(i+1)%3]);
   if ( face == facef )
    return 1 ;
  }

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
  if ( dist > (height/(SPH_FACTOR*1.0)) )
   return 1 ;

  /* if we get here the distance from the new node to the face's plane is not
     bigger than the height of the new element divided by a factor. We should
     then evaluate the projection of the new node into  the  face's plane and
     verify if this point is inside of the face or near of it to reject  this
     new node. The projection of the new node is computed  by a  dot product
     between a vector formed by the new node and the face's center and the nor-
     mal vector of the face. */

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
   if ( dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }
  else if (u > 0.0 && v < 0.0 && w > 0.0)
  {
   edge = sqrt ( (c1[0]-c3[0]) * (c1[0]-c3[0]) +
		 (c1[1]-c3[1]) * (c1[1]-c3[1]) +
		 (c1[2]-c3[2]) * (c1[2]-c3[2]) ) ;
   param = (face->area * 2.0) / edge;
   dist = param * fabs(v);
   if (dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }
  else if (u > 0.0 && v > 0.0 && w < 0.0)
  {
   edge = sqrt ( (c1[0]-c2[0]) * (c1[0]-c2[0]) +
		 (c1[1]-c2[1]) * (c1[1]-c2[1]) +
		 (c1[2]-c2[2]) * (c1[2]-c2[2]) ) ;
   param = (face->area * 2.0) / edge;
   dist = param * fabs(w);
   if (dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }

  if (u < 0.0 && v < 0.0 && w > 0.0)
  {
   dist = sqrt ( (c[0]-c3[0]) * (c[0]-c3[0]) +
		 (c[1]-c3[1]) * (c[1]-c3[1]) +
		 (c[2]-c3[2]) * (c[2]-c3[2]) ) ;
   if (dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }
  else if (u < 0.0 && v > 0.0 && w < 0.0)
  {
   dist = sqrt ( (c[0]-c2[0]) * (c[0]-c2[0]) +
		 (c[1]-c2[1]) * (c[1]-c2[1]) +
		 (c[2]-c2[2]) * (c[2]-c2[2]) ) ;
   if (dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }
  else if (u > 0.0 && v < 0.0 && w < 0.0)
  {
   dist = sqrt ( (c[0]-c1[0]) * (c[0]-c1[0]) +
		 (c[1]-c1[1]) * (c[1]-c1[1]) +
		 (c[2]-c1[2]) * (c[2]-c1[2]) ) ;
   if (dist > (height/(SPH_FACTOR*1.0)))
    return 1 ;
  }

  /* if we get here, then distance is not bigger than height divided by a fac-
     tor and the projection of the new node in the plane of the face is inside
     of the face or near of it, then we should return a invalid status. */

   return 0 ;
}

/*
** ---------------------------------------------------------------------
** InsNewNode - routines to insert new node.
*/

static int InsNewNode (
 Msh3DBdryFace       face,
 Msh3DBdryNodeList   nl,
 int		    *n,
 int                 debug,
 int                *a )
{
 Msh3DBdryFace     facef ;
 int		   j ;
 int		   i ;
 int		   num_node ;
 int               num, alloc ;
 double             dot ;
 double		   node[3] ;
 double             new[3] ;
 double             cand_vec[3] ;

 /* initialize baricent */

 for (j = 0; j < 3; j++)
  node[j] = 0.0;
 num = 0;

 /* initialize number of nodes and allocation */

 num_node = *n;
 alloc    = *a;

 /* add new coordinate for baricent for base face */

 InsNewNodeFindNode (face,debug,new);
 for (j = 0; j < 3; j++)
  node[j] += new[j];
 num++;

 /* add new coordinates for adjacent face */

 for (j = 0; j < 3; j++)
 {
  facef = InsNewNodeFindFace (face,nl,face->verts[j],face->verts[(j+1)%3]);
  if (facef != NULL)
  {
   InsNewNodeFindNode (facef,debug,new);
   for (i = 0; i < 3; i++)
    node[i] += new[i];
   num++;
  }
 }

 /* calculate the new point */

 for (j = 0; j < 3; j++)
  node[j] /= num;

 /* update node_list */

 if (alloc == 1)
 {
  alloc++;
  num_node++;
  nintnode++;
 }
 for (j = 0; j < 3 ; j++)
  nl[num_node-1].coord[j] = node[j];
 nl[num_node-1].active_flag = 1;
 nl[num_node-1].faces = (Msh3DAdjFaceList)0 ;

 /* update num_node and allocation */

 *n = num_node;
 *a = alloc;

 /* the new point should only be considered if it's in the same semi-plane
    than the base face, there is, in the direction of base face's normal */

 cand_vec[0] = node[0] - face->center[0] ;
 cand_vec[1] = node[1] - face->center[1] ;
 cand_vec[2] = node[2] - face->center[2] ;

 dot = cand_vec[0] * face->r_x_s[0] +
       cand_vec[1] * face->r_x_s[1] +
       cand_vec[2] * face->r_x_s[2] ;

 if ( dot <= 0.0 ) return 0 ;
 else              return 1 ;
}

static Msh3DBdryFace InsNewNodeFindFace (
 Msh3DBdryFace      face,
 Msh3DBdryNodeList  node_list,
 int		    fid,
 int		    sid )
{
 Msh3DAdjFaceList face1, aux1 ;
 Msh3DAdjFaceList face2, aux2 ;
 Msh3DBdryFace    facef = NULL ;
 int              found = 0 ;
 int              i, tid ;
 double            cand_vec[3], dot ;

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

 dot = cand_vec[0] * face->r_x_s[0] +
       cand_vec[1] * face->r_x_s[1] +
       cand_vec[2] * face->r_x_s[2] ;

 if ( dot < 0.0 ) return NULL ;
 else             return facef ;
}

static void InsNewNodeFindNode (
 Msh3DBdryFace       face,
 int                 debug,
 double		     node[3] )
{
 double             l ;
 double		    h ;
 double             d ;
 double             nrm[3] ;

 /* get the triangle equilateral size of the same area of the face */

 l = sqrt( 4*face->area / sqrt(3.0) );

 /* evaluate the height */

 h = (l * sqrt(13.0)) / 4.0;

 /* normalize the face normal */

 d = sqrt( face->r_x_s[0] * face->r_x_s[0] +
	   face->r_x_s[1] * face->r_x_s[1] +
	   face->r_x_s[2] * face->r_x_s[2] );
 nrm[0] = face->r_x_s[0] / d;
 nrm[1] = face->r_x_s[1] / d;
 nrm[2] = face->r_x_s[2] / d;

 /* calculate the new node coordinate */

 node[0] = face->center[0] + (h/debug)*nrm[0];
 node[1] = face->center[1] + (h/debug)*nrm[1];
 node[2] = face->center[2] + (h/debug)*nrm[2];
}

