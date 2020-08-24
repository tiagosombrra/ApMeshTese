#ifndef _DATA_PREDICATES_H_
#define _DATA_PREDICATES_H_

#include "Data/Definitions.h"

#if USE_EXACT

#ifdef __cplusplus
namespace Data
{
	namespace Predicates
	{
		extern "C"
		{
#endif //#ifdef __cplusplus

#if USE_DOUBLE
	//#define REAL double
	typedef double REAL;
	#define REALPRINT doubleprint
	#define REALRAND doublerand
	#define NARROWRAND narrowdoublerand
	#define UNIFORMRAND uniformdoublerand
#else
	//#define REAL float
	typedef float REAL;
	#define REALPRINT floatprint
	#define REALRAND floatrand
	#define NARROWRAND narrowfloatrand
	#define UNIFORMRAND uniformfloatrand
#endif //#if USE_DOUBLE

			REAL getfpuprecision ( );

			void doubleprint     ( double number );
			void floatprint      ( float number );
			void expansion_print ( int elen, REAL *e );

			double doublerand        ( );
			double narrowdoublerand  ( );
			double uniformdoublerand ( );

			float floatrand        ( );
			float narrowfloatrand  ( );
			float uniformfloatrand ( );

			void exactinit ( );

			int grow_expansion          ( int elen, REAL *e, REAL b, REAL *h );
			int grow_expansion_zeroelim ( int elen, REAL *e, REAL b, REAL *h );

			int expansion_sum           ( int elen, REAL *e, int flen, REAL *f, REAL *h );
			int expansion_sum_zeroelim1 ( int elen, REAL *e, int flen, REAL *f, REAL *h );
			int expansion_sum_zeroelim2 ( int elen, REAL *e, int flen, REAL *f, REAL *h );

			int fast_expansion_sum          ( int elen, REAL *e, int flen, REAL *f, REAL *h );
			int fast_expansion_sum_zeroelim ( int elen, REAL *e, int flen, REAL *f, REAL *h );

			int linear_expansion_sum          ( int elen, REAL *e, int flen, REAL *f, REAL *h );
			int linear_expansion_sum_zeroelim ( int elen, REAL *e, int flen, REAL *f, REAL *h );

			int scale_expansion          ( int elen, REAL *e, REAL b, REAL *h );
			int scale_expansion_zeroelim ( int elen, REAL *e, REAL b, REAL *h );

			int pcompress ( int elen, REAL *e, REAL *h );

			REAL estimate ( int elen, REAL *e );

			REAL orient2d      ( REAL* pa, REAL* pb, REAL* pc );
			REAL orient2dfast  ( REAL* pa, REAL* pb, REAL* pc );
			REAL orient2dexact ( REAL* pa, REAL* pb, REAL* pc );
			REAL orient2dslow  ( REAL* pa, REAL* pb, REAL* pc );
			REAL orient2dadapt ( REAL *pa, REAL *pb, REAL *pc, REAL detsum );

			REAL orient3d      ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL orient3dfast  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL orient3dexact ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL orient3dslow  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL orient3dadapt ( REAL *pa, REAL *pb, REAL *pc, REAL *pd, REAL permanent );

			REAL incircle      ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL incirclefast  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL incircleexact ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL incircleslow  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd );
			REAL incircleadapt ( REAL *pa, REAL *pb, REAL *pc, REAL *pd, REAL permanent );

			REAL insphere      ( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe );
			REAL inspherefast  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe );
			REAL insphereexact ( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe );
			REAL insphereslow  ( REAL* pa, REAL* pb, REAL* pc, REAL* pd, REAL* pe );
			REAL insphereadapt ( REAL *pa, REAL *pb, REAL *pc, REAL *pd, REAL *pe, REAL permanent );

#ifdef __cplusplus
		}
	}
}
#endif //#ifdef __cplusplus

#endif //#if USE_EXACT

#endif // _DATA_PREDICATES_H_
