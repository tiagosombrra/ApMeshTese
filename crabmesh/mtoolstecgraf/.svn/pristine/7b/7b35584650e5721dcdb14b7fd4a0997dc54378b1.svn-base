/*
** ----------------------------------------------------------------------
**
** JmeshInseKer3D.c - Kernel insertion module.
**
** ----------------------------------------------------------------------
**
** Description:
**  Routines to insert a new node in the kernel of a given polyhedron.
**
** Copyright:
**  (c) 1996 TECGRAF/PUC-Rio && CFG/Cornell University
**           All rights reserved
**
** History:
**  Created:	Oct-01-1996	Joaquim Bento Cavalcante Neto
**   Initial version. Created during my PhD Sandwich in Cornell.
**   It was based on Tilio's implementation for kernel insertion.
**  Modified:	Oct-25-2001	Joaquim Bento Cavalcante Neto
**   This procedure, while still being kept in the code for the sake
**   of documentation, it is not being used anymore because it does
**   not guarantee improvement on the algorithm for mesh generation.
**  Modified:	Oct-23-2003	Joaquim Bento Cavalcante Neto
**   Modified to get rid of all remaining compiler's warnings.
**
** ----------------------------------------------------------------------
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define EPS 1.0e-06
#define FREEALL free_ivector(l3,1,m);free_ivector(l2,1,m);\
                free_ivector(l1,1,n+1);

#define NEQS 50

/*
** ---------------------------------------------------------------------
** Local Functions Prototypes:
*/

static void simp1 ( float a[NEQS][NEQS], int mm, int ll[], int nll, int iabf,
                    int *kp, float *bmax );
static void simp2 ( float a[NEQS][NEQS], int n, int l2[], int nl2, int *ip,
                    int kp, float *q1 );
static void simp3 ( float a[NEQS][NEQS], int i1, int k1, int ip, int kp );

/*
** ---------------------------------------------------------------------
** Local Functions:
*/

static void nerror( char s[] )
{
  fprintf(stdout,"%s\n",s);
}

static void free_ivector( int *v, int m, int n )
{
 /* free((char*)v+m);*/
}

static int *ivector( int m, int n )
{
  int *v;

  v = (int*)calloc(n-m+1,sizeof(int));
  if (!v)nerror("allocation failure\n") ;
  return v-m;
}

/*  Determines the maximum of those elements whose index is contained
**  in the supplied list ll, either with or without taking the absolute
**  value, as flagged by iabf. */

static void simp1(
  float a[NEQS][NEQS],
  int mm,
  int ll[],
  int nll,
  int iabf,
  int *kp,
  float *bmax )
{
  int k;
  float test;

  *kp = ll[1];
  *bmax = a[mm+1][*kp+1];
  for( k=2; k<=nll; k++) {
    if ( iabf == 0 )
      test = a[mm+1][ll[k]+1]-(*bmax);
    else
      test = fabs(a[mm+1][ll[k]+1])-fabs(*bmax);
    if( test > 0.0 ) {
      *bmax = a[mm+1][ll[k]+1];
      *kp = ll[k];
    }
  }
}

/*  Locate a pivot element, taking degeneracy into account */

static void simp2(
  float a[NEQS][NEQS],
  int n,
  int l2[],
  int nl2,
  int *ip,
  int kp,
  float *q1 )
{
  int k, ii, i;
  float qp,q0,q;

  *ip = 0;
  for ( i=1; i<= nl2; i++ ){
    if ( a[l2[i]+1][kp+1] < -EPS ) {
      *q1 = -a[l2[i]+1][1]/a[l2[i]+1][kp+1];
      *ip = l2[i];
      for ( i=i+1; i<=nl2; i++ ) {
        ii=l2[i];
        if ( a[ii+1][kp+1] < -EPS ) {
          q = -a[ii+1][1]/a[ii+1][kp+1];
          if( q < *q1 ) {
            *ip = ii;
            *q1 = q;
          } else if ( q == *q1 ) {  /* we have a degeneracy */
            for ( k=1; k<=n; k++ ) {
              qp = -a[*ip+1][k+1]/a[*ip+1][kp+1];
              q0 = -a[ii+1][k+1]/a[ii+1][kp+1];
              if ( q0 != qp ) break;
            }
            if ( q0 < qp ) *ip=ii;
          }
        }
      }
    }
  }                    /* no possible pivots. return with message */
}

/*  Matrix operations to exchange a left- and right-hand variable */

static void simp3( a, i1, k1, ip, kp )
  float a[NEQS][NEQS];
  int i1, k1, ip, kp;
{
  int kk,ii;
  float piv;

  piv = 1.0/a[ip+1][kp+1];
  for ( ii=1; ii<=i1+1; ii++ )
    if ( ii-1 != ip ) {
      a[ii][kp+1] *= piv;
      for ( kk=1; kk<=k1+1; kk++ )
        if ( kk-1 != kp )
          a[ii][kk] -= a[ip+1][kk]*a[ii][kp+1];
    }
  for ( kk=1; kk<=k1+1; kk++ )
    if ( kk-1 != kp )
      a[ip+1][kk] *= -piv;
  a[ip+1][kp+1] = piv;
}

/*
** ---------------------------------------------------------------------
** Public Functions:
*/

void simplx (
  float a[NEQS][NEQS],
  int m,
  int n,
  int m1,
  int m2,
  int m3,
  int *icase,
  int izrov[],
  int iposv[] )
{
  int     i, j, ip, ir, is, k, kh, kp, m12, nl1, nl2;
  int    *l1, *l2, *l3, *ivector();
  float   q1, bmax;
  void simp1(), simp2(), simp3(), nerror(), free_ivector();

#if 0
  fprintf(stdout, "test \n");
  for(i=1;i<m+3;i++) {
   for(j=1;j<n+2;j++)
     fprintf(stdout, "%f  ",a[i][j]);
   fprintf(stdout, "\n");
  }
#endif

  if ( m != (m1+m2+m3) )
    nerror("bad input constraint counts in SIMPLX");
  l1 = ivector(1,n+1);
  l2 = ivector(1,m);
  l3 = ivector(1,m);
  nl1 = n;                 /* initially make all variables ritgh-hand */
  for ( k=1; k<=n; k++ )   /* initialize index lists */
    l1[k] = izrov[k] = k;
  nl2 = m;                 /* make all artificial variables left-hand,
                              and initialize those lists */
  for ( i=1; i<=m; i++ ) {
    if ( a[i+1][1] < 0.0 ) { /* constants bi must be nonnegative */
      fprintf(stdout,"at line:%d  %f\n",i, a[i+1][1]);
      nerror("bad input tableau in SIMPLX");
    }
    l2[i] = i;
    iposv[i]=n+1;
  }
  for ( i=1; i<=m2; i++ )
    l3[i]=1;               /* used later but initialized here */

  ir = 0;
/*  this flag setting means we are in phase two, i.e have a
**  feasible starting solution. Go to phase two if origin is a
**  feasible solution */

  if ( m2+m3 ) {
    ir=1;                 /* flag meaning that we must start out
                             in phase one */
    for ( k=1; k<=(n+1); k++ ) {   /* compute the auxiliary
                                      objective function */
      q1 = 0.0;
      for ( i=m1+1; i<=m; i++ )
        q1 += a[i+1][k];
      a[m+2][k] = -q1;
    }
    do {

/* Find max. coeff. of auxiliary objective fn. */
      simp1( a, m+1, l1, nl1, 0, &kp, &bmax);

      if ( bmax <= EPS && a[m+2][1] < -EPS ) {

        *icase = -1;       /* auxiliary objective function is still
                              negative and can't be improved, hence
                              no feasible solution exists */
        FREEALL return;

      } else if ( bmax <= EPS && a[m+2][1] <= EPS ) {
        m12 = m1+m2+1;     /* auxiliary objective function is zero
                              negative and can't be improved. This
                              signals that we have a feasible starting
                              vector. */
        if ( m12 <= m ) {

          for ( ip=m12; ip<=m; ip++ ) {
 /* clean out the artificial variables by "goto one" and then
    move on to phase two */

            if ( iposv[ip] == (ip+n) ) {
              simp1( a, ip, l1, nl1, 1, &kp, &bmax );
              if( bmax > 0.0 )
                goto one;
            }
          }
        }
        ir=0;              /* set flag indicating we have reached
                              phase two */
        --m12;
        if ( m1+1 <= m12 )
          for ( i=m1+1; i<=m12; i++ )
            if ( l3[i-m1] == 1 )
              for ( k=1; k<=n+1; k++ )
                a[i+1][k] = -a[i+1][k];
        break;             /* go to phase two */
      }

      simp2( a, n, l2, nl2, &ip, kp, &q1 );  /* locate a pivot element
                                                (phase one) */
      if ( ip == 0 ) {     /* maximum of auxiliary objective function
                              is unbounded, so no feasible solution
                              exists */
        *icase=-1;
        FREEALL return;
      }
one:  simp3( a, m+1, n, ip, kp );  /* exchange a left- and a right-hand
                                      variable(phase one), then update
                                      lists */
      if ( iposv[ip] >= (n+m1+m2+1) ) {
        for ( k=1; k<=nl1; k++ )
          if ( l1[k]==kp )
            break;
        --nl1;
        for ( is=k; is<=nl1; is++ )
          l1[is] = l1[is+1];
        a[m+2][kp+1] += 1.0;
        for ( i=1; i<=m+2; i++ )
          a[i][kp+1] = -a[i][kp+1];

      } else {
        if ( iposv[ip] >= (n+m1+1) ) {
          kh = iposv[ip]-m1-n;
          if ( l3[kh] ) {
            l3[kh] = 0;
            a[m+2][kp+1] += 1.0;
            for( i=1; i<=m+2; i++ )
              a[i][kp+1] = -a[i][kp+1];
          }
        }
      }
      is = izrov[kp];
      izrov[kp] = iposv[ip];
      iposv[ip] = is;

    } while (ir);          /* is still in phase one, go back to the do */
  }

/*  end of phase one code for finding an initial feasible solution.
    Now, in phase two, optimize it. */

  for (;;) {

    simp1( a, 0, l1, nl1,0, &kp, &bmax ); /* test the z-row for doneness */
    if ( bmax <= 0.0 ) {   /* done. solution found.
                              return with the good news */
      *icase=0;
      FREEALL return;
    }
    simp2( a, n, l2, nl2, &ip, kp, &q1 ); /* locate a pivot element
                                             (phase two) */
    if ( ip==0 ) {         /* objective function is unbounded.
                              report and return */
      *icase=1;
      FREEALL return;
    }
    simp3( a, m, n, ip, kp );   /* exchange a left- and right-hand
                                   variable (phase two) */
    is = izrov[kp];
    izrov[kp] = iposv[ip];
    iposv[ip] = is;
  }                             /* and return for another iteration */
}

#if 0
  fprintf(stdout, "\n");
  fprintf(stdout, "Phase two \n");
  for(i=1;i<m+3;i++) {
   for(j=1;j<n+2;j++)
     fprintf(stdout, "%f  ",a[i][j]);
   fprintf(stdout, "\n\n\n");
  }
#endif

typedef struct _GeoPoint { float x, y, z ; }   	    GeoPoint ;
typedef struct _GeoPlanEqn { float a, b, c, d ; }   GeoPlanEqn ;
typedef struct _TriFace { int p1, p2, p3 ; }  TriFace ;

static float    objfunc[100];
static GeoPoint nlist[50];
static TriFace  flist[50];
static int      nobjfuncs, nnodes, nfaces;
static float    **tableau;

#define TOL 1e-05

#define iszero(n)  (n<TOL && n>-TOL)

static void genplaneqn(
    GeoPoint	*pt0, GeoPoint *pt1, GeoPoint *pt2,
    GeoPlanEqn	*plan_eqn )
{
    GeoPoint	vec1, vec2, normal ;
    float	length ;

    vec1.x =  pt1->x - pt0->x ;
    vec1.y =  pt1->y - pt0->y ;
    vec1.z =  pt1->z - pt0->z ;
    vec2.x =  pt2->x - pt0->x ;
    vec2.y =  pt2->y - pt0->y ;
    vec2.z =  pt2->z - pt0->z ;

    normal.x = (vec1.y * vec2.z) - (vec1.z * vec2.y) ;
    normal.y = (vec1.z * vec2.x) - (vec1.x * vec2.z) ;
    normal.z = (vec1.x * vec2.y) - (vec1.y * vec2.x) ;

    length = sqrt(     normal.x*normal.x
		    +  normal.y*normal.y
		    +  normal.z*normal.z  ) ;

    plan_eqn->a = normal.x / length ;
    plan_eqn->b = normal.y / length ;
    plan_eqn->c = normal.z / length ;
    plan_eqn->d = (  - plan_eqn->a * pt0->x
		     - plan_eqn->b * pt0->y
		     - plan_eqn->c * pt0->z  ) ;
}

void IniSimplex (int cnt)
{
  int i;

  nobjfuncs = 8;
  objfunc[0]  =  1; objfunc[1]  =  1; objfunc[2]  =  1;
  objfunc[3]  =  1; objfunc[4]  =  1; objfunc[5]  = -1;
  objfunc[6]  = -1; objfunc[7]  =  1; objfunc[8]  =  1;
  objfunc[9]  =  1; objfunc[10]  = -1; objfunc[11] =  1;
  objfunc[12] = -1; objfunc[13] = -1; objfunc[14] = -1;
  objfunc[15] = -1; objfunc[16] = -1; objfunc[17] =  1;
  objfunc[18] =  1; objfunc[19] = -1; objfunc[20] = -1;
  objfunc[21] = -1; objfunc[22] =  1; objfunc[23] = -1;

  nfaces = cnt;

  tableau = (float**)calloc(nfaces+3,sizeof(float*));
  for(i=0;i<nfaces+3;i++)
    tableau[i] = (float*)calloc(4,sizeof(float));
}

void  AddFacetoTableau (
 int    cnt,
 float  v1x, float v1y, float v1z,
 float  v2x, float v2y, float v2z,
 float  v3x, float v3y, float v3z )
{
  int         i;
  GeoPlanEqn  planeqn;
  GeoPoint    pt[3];

  pt[0].x = v1x;  pt[0].y = v1y;  pt[0].z = v1z;
  pt[1].x = v2x;  pt[1].y = v2y;  pt[1].z = v2z;
  pt[2].x = v3x;  pt[2].y = v3y;  pt[2].z = v3z;

  genplaneqn( &pt[0], &pt[1], &pt[2], &planeqn );
  planeqn.a = iszero(planeqn.a)?0:planeqn.a;
  planeqn.b = iszero(planeqn.b)?0:planeqn.b;
  planeqn.c = iszero(planeqn.c)?0:planeqn.c;
  planeqn.d = iszero(planeqn.d)?0:planeqn.d;

#if 0
    fprintf(stdout, "a = %f\nb = %f\nc = %f\nd = %f\n",
                     planeqn.a, planeqn.b, planeqn.c,
                     planeqn.d);
#endif

  tableau[cnt+2][1] = planeqn.d;
  tableau[cnt+2][2] = planeqn.a;
  tableau[cnt+2][3] = planeqn.b;
  tableau[cnt+2][4] = planeqn.c;
}

float *GetNewPoint (void)
{
  int    i,j,k,n,m,m1,m2,m3;
  int    icase, izrov[NEQS], iposv[NEQS];
  float  a[NEQS][NEQS], r[NEQS], *pt;

  n = 3;
  m2 = m3 = 0;
  m = m1 = nfaces;
  pt = (float*)calloc(n+1,sizeof(float));
  for(i=1;i<=n;i++)
    pt[i] = 0.0;

/* last row of matrix A */

  for(i=1;i<n+2;i++)
    tableau[m+2][i] = 0.0;

  fprintf(stdout, "input \n");
  for(i=2;i<m+3;i++) {
   for(j=1;j<n+2;j++)
     fprintf(stdout, "%f  ",tableau[i][j]);
   fprintf(stdout, "\n");
  }


  for(k=0;k<nobjfuncs;k++) {

/* first row of matrix A */
    tableau[1][1] = 0.0;
    for(i=2;i<n+2;i++)
      tableau[1][i] = objfunc[3*k+i-2];

   for(i=1;i<m+3;i++)
     for(j=1;j<n+2;j++)
       a[i][j] = tableau[i][j];

    simplx(a,m,n,m1,m2,m3,&icase,izrov,iposv);

#if 0
  fprintf(stdout, "icase: %d\n",icase);

  fprintf(stdout, "iposv: ");
  for(i=1;i<=m;i++)
    fprintf(stdout, "%d ",iposv[i]);
  fprintf(stdout, "\n");

  fprintf(stdout, "izrov: ");
  for(i=1;i<=n;i++)
    fprintf(stdout, "%d ",izrov[i]);
  fprintf(stdout, "\n\n");

  for(i=1;i<m+3;i++) {
   for(j=1;j<n+2;j++)
     fprintf(stdout, "%f  ",a[i][j]);
   fprintf(stdout, "\n");
  }
#endif

    for(i=1;i<=n;i++)
      r[i] = 0.0;

    for(i=1;i<=m;i++)
      if (iposv[i]<=n)
        r[iposv[i]] = a[i+1][1];

    fprintf(stdout, "\nobjfunc: x1 = %f  x2 = %f  x3 = %f\n\n",
                     objfunc[3*k+0], objfunc[3*k+1],
                     objfunc[3*k+2] );
    for(i=1;i<=n;i++) {
      fprintf(stdout, "x[%d] = %f\n", i,r[i]);
      pt[i] += r[i];
    }
  }
  for(i=1;i<=n;i++)
    pt[i] /= n;

  return pt;
}
