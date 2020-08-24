/*
** ----------------------------------------------------------------------
**
** JmeshMainDrive.c - Main driver application module.
**
** ----------------------------------------------------------------------
**
** Description:
**  This file implements the main driver for a JMESH application.
**  To use:  jmesh (input_format) (jmesh_version) < (input) > (output)
**  Example: jmesh -dmesh_fmt -advan_ver < example.dmsh > example.dout
**  Formats: -dmesh_fmt, -jmesh_fmt and -wmesh_fmt
**  Version: -advan_ver and -small_ver
**  Default: -dmesh_fmt and -advan_ver
**  Observation: the defaults do not need to be typed.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**  Modified:	Dec-09-1999	Joaquim Bento Cavalcante Neto
**   Include new variable version to define the jmesh version to be used.
**   This version can be SMALL or ADVAN depending on the case. The default
**   version is the ADVAN one represented by the define ADVAN_VERSION.
**  Modified:	Oct-25-2001	Joaquim Bento Cavalcante Neto
**   Changed the use of variable fac to allow more control over the degree
**   of refinement of the mesh.
**  Modified:	Aug-01-2002	Joaquim Bento Cavalcante Neto
**   Changed the name of all files. The Jmesh related files start now
**   with Jmesh, instead of msh, and all Tecgraf files with Tecgraf.
**   Created a new variable format to deal with the different versions of
**   Jmesh's input. The version is defined in run-time by the following:
**    a) Default   format: -dmesh_fmt
**    b) Joaquim's format: -jmesh_fmt
**    c) Wash's    format: -wmesh_fmt
**   To run Jmesh one must do:
**    a) Default   format: jmesh -dmesh_fmt < in.dmsh > out.dout
**    b) Joaquim's format: jmesh -jmesh_fmt < in.jmsh > out.jout
**    c) Wash's    format: jmesh -wmesh_fmt < in.wmsh > out.wout
**   The default format do not need to be typed and it is the format that
**   is assumed if no one is given. Currently the versions supported are:
**    a) Default   format:
**       <factor>   - refinement factor for octree generation (use 0.4)
**       <tol>      - tolerance for validity geometric checks (use 0.001)
**       <internal> - use (1/0) of octree for internal nodes  (use 1)
**       <fromone>  - start (1/0) in node numbering for faces (use 1)
**       <n> - number of boundary nodes
**       <node1_x>  <node1_y>  <node1_z>
**       ...
**       <noden_x>  <noden_y>  <noden_z>
**       <m> - number of triangular boundary facets
**       <face1_node1> <face1_node2> <face1_node3>
**       ...
**       <facem_node1> <facem_node2> <facem_node3>
**    b) Joaquim's format:
**       <factor>   - refinement factor for octree generation (use 0.4)
**       <tol>      - tolerance for validity geometric checks (use 0.001)
**       <internal> - use (1/0) of octree for internal nodes  (use 1)
**       <fromone>  - start (1/0) in node numbering for faces (use 1)
**       <n> - number of boundary nodes
**       <node1_id> <node1_x>  <node1_y>  <node1_z>
**       ...
**       <noden_id> <noden_x>  <noden_y>  <noden_z>
**       <m> - number of triangular boundary facets
**       <face1_node1> <face1_node2> <face1_node3>
**       ...
**       <facem_node1> <facem_node2> <facem_node3>
**    c) Wash's    format:
**       <n> <m> - number of nodes and faces in that order
**       <node1_id> <node1_x>  <node1_y>  <node1_z>
**       ...
**       <noden_id> <noden_x>  <noden_y>  <noden_z>
**       <face1_id> <face1_node1> <face1_node2> <face1_node3>
**       ...
**       <facem_id> <facem_node1> <facem_node2> <facem_node3>
**   Some considerations are important:
**    a) Default   format:
**       The node numbering can start from 0 or 1. If starts from 0, the
**       ids of the vertices used by the faces MUST start from 0. Similar
**       rule applies if starts from 1, that is, the ids of the faces MUST
**       start from 1 in that case. Also the triangular faces that define
**       the boundary should point towards to the interior of the model,
**       that is, their normals should point into the region being meshed
**       in a left-hand sense.
**    b) Joaquim's format:
**       The same considerations of the default format apply here.
**    c) Wash's    format:
**       The node numbering should start from 1. The face numbering should
**       start from 0. This is not an obligation, since it can start from
**       any number and it is not necessary to have the ids of the nodes
**       in a consecutive order. Also the triangular faces that define the
**       boundary should point towards to the exterior of the model, that
**       is, they should point into the region being meshed in a right-hand
**       sense. This is the opposite from the other two formats.
**   All observations made here are valid for both versions of Jmesh, that
**   is, the C version (Joaquim) and C++ version (Wash). Below follows an
**   example in each format:
**    a) Default   format:
**       0.4
**       0.001
**       1
**       1
**       4
**       0.0     0.0     0.0
**       1.0     0.0     0.0
**       0.0     1.0     0.0
**       0.0     0.0     1.0
**       4
**       1       2       3
**       1       3       4
**       2       1       4
**       3       2       4
**    b) Joaquim's format:
**       0.4
**       0.001
**       1
**       1
**       4
**       1       0.0     0.0     0.0
**       2       1.0     0.0     0.0
**       3       0.0     1.0     0.0
**       4       0.0     0.0     1.0
**       4
**       1       2       3
**       1       3       4
**       2       1       4
**       3       2       4
**    c) Wash's    format:
**       4       4
**       1       0.0     0.0     0.0
**       2       1.0     0.0     0.0
**       3       0.0     1.0     0.0
**       4       0.0     0.0     1.0
**       0       1       3       2
**       1       1       4       3
**       2       2       4       1
**       3       3       4       2
**  Modified:	Aug-02-2002	Joaquim Bento Cavalcante Neto
**   Instead of printing the generated mesh in a file called jmesh.jpos,
**   the generated mesh is printed out in the stdout. If the user wants
**   to store it in a file, it should define in the beginning by the
**   following rule: [jmesh < (in.fmt) > (out.fmt)] or it should change
**   the variable file, which it is hard-coded, to be 1.
**  Modified:	Jan-29-2003	Joaquim Bento Cavalcante Neto
**   Changed name of the file from JmeshTestDrive.c to JmeshMainDrive.c.
**   Also, included the possibility to define the version (ADVAN or SMALL)
**   in run-time, like what's being done with the formats. If no version
**   is given, ADVAN one is assumed, represented by define ADVAN_VERSION.
**   The SMALL version is recommended in case of transition problems for
**   the triangular facets on the surface mesh, mainly in case of cracks.
**   To run Jmesh (with the SMALL version) one must do:
**    a) Default   format: jmesh -dmesh_fmt -small_ver < in.dmsh > out.dout
**    b) Joaquim's format: jmesh -jmesh_fmt -small_ver < in.jmsh > out.jout
**    c) Wash's    format: jmesh -wmesh_fmt -small_ver < in.wmsh > out.wout
**  Modified:	Mar-26-2003	Joaquim Bento Cavalcante Neto
**   Modified look-up table process to speed up. The old version for the
**   look-up table can be retrieved by turning #define OLD_LOOK to 1.
**   Also created another #define, called OLD_DEBU, for debug purposes
**   as well as the define #DSP_PRNT, to print message of generation.
**   Created #defines VER_BOTH and VER_PRNT to use both Jmesh versions.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ----------------------------------------------------------------------
*/

#include "Jmesh/JmeshMainDef3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//commented by Markos
//#define SMALL_VERSION 	0
//#define ADVAN_VERSION 	1
//end commented by Markos

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC  1.0e+06
#endif

/* --------------------------------------------------------------
** Definitons for use of algorithms:
*/

#define DSP_TIME	1	/* Print the execution time */
#define DSP_PRNT	1	/* Print messages for jmesh */

#define OLD_LOOK	0	/* Old look-up table process   */
#define OLD_DEBU	0	/* Debug for new look-up table */

#define VER_BOTH        1       /* Try Jmesh with both versions */
#define VER_PRNT        1       /* Print Jmesh warning messages */

/* ----------------------------------------------------------------------
** Public function:
*/

int main_original( int argc, char *argv[] )
{
    int     internal, fromone, version, format ;
    int     num_node, num_face, i, j, k, max = -1 ;
    int     offset = 0 ;
    double  **nodes, factor, tol ;
    int     *tnodes = NULL ;
    int     *rnodes = NULL ;
    int     **faces, rnode ;
    int     num_gen_nodes = 0;
    double  *generated_nodes ;
    int     *tgenerated_nodes = NULL ;
    int     num_elems, *elems ;
    int     status, number, out = 1 ;
    char    *text ;
    int     file = 0 ;
    FILE    *fp ;
    long    init_time, tot_time ;
    long    init_gbl_time, tot_gbl_time ;

    /* Init process */

    fprintf (stderr, "\n" );
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\t                  J M E S H                      \n" );
    fprintf (stderr, "\t  THREE-DIMENSIONAL TETRAHEDRAL MESH GENERATOR   \n" );
    fprintf (stderr, "\t    TECGRAF/PUC-Rio & CFG/Cornell University     \n" );
    fprintf (stderr, "\t                 October/2003                    \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t             Joaquim Cavalcante-Neto             \n" );
    fprintf (stderr, "\t                 Wash Wawrzynek                  \n" );
    fprintf (stderr, "\t                  Bruce Carter                   \n" );
    fprintf (stderr, "\t               Luiz Fernando Martha              \n" );
    fprintf (stderr, "\t                Anthony Ingraffea                \n" );
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\n" );

    fprintf (stderr, "Model Creation:") ;
    fprintf( stderr, "\n" );

#if DSP_TIME
    init_time = clock( );
    init_gbl_time = clock( );
    fprintf( stderr, "\n" );
    fprintf( stderr, "	Building look-up table................" );
    fprintf( stderr, "\n" );
#endif

    /* Default values */

    version  = ADVAN_VERSION ;
    format   = 0 ;
    factor   = 0.4 ;
    tol      = 0.001 ;
    internal = 1 ;
    fromone  = 1 ;

    /* Read the format, if given */

    for( i = 1; i < argc; i++ )
    {
     /* Read the format */
     if (strcmp("-dmesh_fmt",argv[i]) == 0)
      format = 0 ;
     if (strcmp("-jmesh_fmt",argv[i]) == 0)
      format = 1 ;
     if (strcmp("-wmesh_fmt",argv[i]) == 0)
      format = 2 ;
     /* Read the version */
     if (strcmp("-advan_ver",argv[i]) == 0)
      version = ADVAN_VERSION ;
     if (strcmp("-small_ver",argv[i]) == 0)
      version = SMALL_VERSION ;
    }

    /* Read the input data */

    switch( format )
    {
     case 0: /* Default  format */
      scanf( "%lf", &factor ) ;
      scanf( "%lf", &tol ) ;
      scanf( "%d",  &internal ) ;
      scanf( "%d",  &fromone ) ;

      scanf( "%d", &num_node ) ;
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ ) {
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       tnodes[i] = i;
      }

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       if( fromone )
       {
        faces[i][0]--;
        faces[i][1]--;
        faces[i][2]--;
       }
      }
      break;

     case 1: /* Joaquim's format */
      scanf( "%lf", &factor ) ;
      scanf( "%lf", &tol ) ;
      scanf( "%d",  &internal ) ;
      scanf( "%d",  &fromone ) ;

#if OLD_LOOK
      scanf( "%d", &num_node ) ;
      nodes  = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       for( k = 0; k < 3; k++ ) {
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          faces[i][k] = j ;
          break ;
         }
        }
       }
      }
#else
      scanf( "%d", &num_node ) ;
      nodes  = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;            /* Normal look-up table */
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      rnode = max+1 ;
      rnodes = (int *)Msh3DCalloc( rnode, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       rnodes[tnodes[i]] = i ;
      }

      scanf( "%d", &num_face ) ;
      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d %d %d", &faces[i][0], &faces[i][1], &faces[i][2] ) ;
       for( k = 0; k < 3; k++ ) {
#if OLD_DEBU
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          rnode = j ;
          break ;
         }
        }
	if (rnode != rnodes[faces[i][k]])
         fprintf (stderr, "Error on look-up table process\n") ;
#endif
	faces[i][k] = rnodes[faces[i][k]] ;
       }
      }
#endif
      break;

     case 2: /* Wash's format */
      scanf( "%d %d", &num_node, &num_face ) ;

#if OLD_LOOK
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d", &j ) ;
       scanf( "%d %d %d", &faces[i][0], &faces[i][2], &faces[i][1] ) ;
       for( k = 0; k < 3; k++ ) {
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          faces[i][k] = j ;
          break ;
         }
        }
       }
      }
#else
      nodes = (double **)Msh3DCalloc( num_node, sizeof(double *) ) ;
      for( i = 0; i < num_node; i++ )
       nodes[i] = (double *)Msh3DCalloc( 3, sizeof(double) ) ;
      tnodes = (int *)Msh3DCalloc( num_node, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       scanf( "%d", &j ) ;
       tnodes[i] = j ;            /* Normal look-up table */
       scanf( "%lf %lf %lf", &nodes[i][0], &nodes[i][1], &nodes[i][2] ) ;
       if( j > max )
        max = j ;
      }
      offset = max - num_node + 1 ;

      rnode = max+1 ;
      rnodes = (int *)Msh3DCalloc( rnode, sizeof(int) ) ;
      for ( i=0 ; i<num_node ; i++ )
      {
       rnodes[tnodes[i]] = i ;
      }

      faces = (int **)Msh3DCalloc( num_face, sizeof(int *) ) ;
      for ( i=0 ; i<num_face ; i++ )
       faces[i] = (int *)Msh3DCalloc( 3, sizeof(int) ) ;
      for ( i=0 ; i<num_face ; i++ )
      {
       scanf( "%d", &j ) ;
       scanf( "%d %d %d", &faces[i][0], &faces[i][2], &faces[i][1] ) ;
       for( k = 0; k < 3; k++ ) {
#if OLD_DEBU
        for( j = 0; j < num_node; j++ ) {
         if( faces[i][k] == tnodes[j] ) {
          rnode = j ;
          break ;
         }
        }
	if (rnode != rnodes[faces[i][k]])
         fprintf (stderr, "Error on look-up table process\n") ;
#endif
	faces[i][k] = rnodes[faces[i][k]] ;
       }
      }
#endif
      break;

     default:
      break;
    }

#if DSP_TIME
    fprintf( stderr,"\n") ;
    tot_time = (clock( ) - init_time)/CLOCKS_PER_SEC;
    fprintf( stderr,"\t\t Initial front size: %8d fac.\n", num_face);
    fprintf( stderr,"\t\t Total elapsed time: %8.2f sec.", (double)tot_time);
    fprintf( stderr,"\n") ;
#endif

    /* Generate the mesh */

#if VER_BOTH
    status = Msh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems,
                              0, NULL ) ; //two last parameters added by markos

    if (status == 0) {
#if VER_PRNT
     fprintf (stderr, "\n");
     fprintf (stderr, "\n");
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\t                  J M E S H                       \n");
     fprintf (stderr, "\tWARNING:  The volume mesh could not be generated!!\n");
     fprintf (stderr, "\tIt is probably related  to problems in the surface\n");
     fprintf (stderr, "\tmesh,  regarding the input requirements for JMESH.\n");
     fprintf (stderr, "\tFor more details,  please see JMESH documentation.\n");
     fprintf (stderr, "\t--------------------------------------------------\n");
     fprintf (stderr, "\n");
#endif
     exit (1) ;
    }
#else
    status = Msh3DGeneration( version, internal, factor, tol,
                              offset, num_node, num_face,
                              nodes, tnodes, faces, &num_gen_nodes,
                              &generated_nodes, &tgenerated_nodes,
                              &num_elems, &elems, 0 ) ; //last parameter added by markos
#endif

    /* Write the generated mesh */

    if( out )
    {
     if( status )
     {
      /* Open file */

      if( file )
       fp = fopen ("jmesh.jpos", "w");
      else
       fp = stdout;

      /* Print data */

      switch( format )
      {
       case 0: /* Default  format */
        fprintf (fp,"%%HEADER\n");
        fprintf (fp,"File created by Jmesh \n");
        fprintf (fp,"\n");
        fprintf (fp,"%%NODE\n%d\n\n",num_node+num_gen_nodes);
        fprintf (fp,"%%NODE.COORD\n%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", i+1,
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", i+1,
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"\n");
	fprintf (fp,"%%INTEGRATION.ORDER\n1\n");
	fprintf (fp,"1\t1\t1\t1\t1\t1\t1\n\n");
        fprintf (fp,"%%ELEMENT\n%d\n\n",num_elems);
        fprintf (fp,"%%ELEMENT.TETR4\n%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, 1, 1,
            elems[i*4+1]+1, elems[i*4+0]+1, elems[i*4+2]+1, elems[i*4+3]+1);
        }
        fprintf (fp,"\n");
        fprintf(fp,"%%END\n");
        break;

       case 1: /* Joaquim's format */
        fprintf (fp,"%%HEADER\n");
        fprintf (fp,"File created by Jmesh \n");
        fprintf (fp,"\n");
        fprintf (fp,"%%NODE\n%d\n\n",num_node+num_gen_nodes);
        fprintf (fp,"%%NODE.COORD\n%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tnodes[i],
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tgenerated_nodes[i-num_node],
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"\n");
	fprintf (fp,"%%INTEGRATION.ORDER\n1\n");
	fprintf (fp,"1\t1\t1\t1\t1\t1\t1\n\n");
        fprintf (fp,"%%ELEMENT\n%d\n\n",num_elems);
        fprintf (fp,"%%ELEMENT.TETR4\n%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\n", i+1, 1, 1,
            elems[i*4+1], elems[i*4+0], elems[i*4+2], elems[i*4+3]);
        }
        fprintf (fp,"\n");
        fprintf(fp,"%%END\n");
        break;

       case 2: /* Wash's format */
        fprintf (fp,"%d\n",num_node+num_gen_nodes);
        for (i = 0; i < (num_node+num_gen_nodes); i++)
        {
         if( i < num_node )
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tnodes[i],
                                          nodes[i][0],
   					  nodes[i][1],
   					  nodes[i][2]);
         }
         else
         {
          fprintf (fp,"%d\t%f\t%f\t%f\n", tgenerated_nodes[i-num_node],
                                          generated_nodes[(i-num_node)*3+0],
   	 				  generated_nodes[(i-num_node)*3+1],
 					  generated_nodes[(i-num_node)*3+2]);
         }
        }
        fprintf (fp,"%d\n",num_elems);
        for( i = 0; i < num_elems; i++)
        {
         fprintf (fp,"%d\t%d\t%d\t%d\t%d\n", i+1,
            elems[i*4+1], elems[i*4+0], elems[i*4+2], elems[i*4+3]);
        }
        fprintf (fp,"\n");
        break;

       default:
        break;
      }

      /* Close file */

      if( file )
       fclose(fp);
     }
    }
    else
    {
     fprintf( stderr, "Status, Num elems : %d %d\n", status, num_elems ) ;
     if ( !status ) {
         fprintf( stderr, "Error status returned \n" ) ;
         Msh3DError( &number, &text ) ;
         fprintf( stderr, "%d %s\n", number, text ) ;
     }
    }

    /* Write the results */

#if DSP_PRNT
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\t                  J M E S H                      \n" );
    fprintf (stderr, "\t  THREE-DIMENSIONAL TETRAHEDRAL MESH GENERATOR   \n" );
    fprintf (stderr, "\t    TECGRAF/PUC-Rio & CFG/Cornell University     \n" );
    fprintf (stderr, "\t                 October/2003                    \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t             Joaquim Cavalcante-Neto             \n" );
    fprintf (stderr, "\t                 Wash Wawrzynek                  \n" );
    fprintf (stderr, "\t                  Bruce Carter                   \n" );
    fprintf (stderr, "\t               Luiz Fernando Martha              \n" );
    fprintf (stderr, "\t                Anthony Ingraffea                \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\t           VOLUME MESH GENERATED!!!              \n" );
    fprintf( stderr,"\t      Generated elements: %8d ele.  \n", num_elems);
#if DSP_TIME
    tot_gbl_time = (clock( ) - init_gbl_time)/CLOCKS_PER_SEC;
    if (tot_gbl_time >= 0)
     fprintf( stderr,"\t      Total elapsed time: %8.2f sec.\n",
		                                     (double)tot_gbl_time);
#endif
    fprintf (stderr, "\t ----------------------------------------------  \n" );
    fprintf (stderr, "\n" );
    fprintf (stderr, "\n" );
#endif

    return (0) ;
}
