//#include <iostream>
//using namespace std;

//#include "Modelo.h"
//#include "Vertice.h"
//#include "CurvParamHermite.h"
//#include "HermitePatch.h"

//double DELTA = 0.001;
//double TOLERANCIA = 0.0001;

//int main ( )
//{
//	Modelo M;
	
//	Ponto* p1 = new Vertice ( -10.0, -5.0, 3.0 );
//	Ponto* p2 = new Vertice (  10.0, -5.0, 3.0 );
//	Ponto* p3 = new Vertice ( -10.0,  5.0, 3.0 );
//	Ponto* p4 = new Vertice (  10.0,  5.0, 3.0 );
	
//	Vetor* v1 = new Vetor (  1.0,  0.0,  0.0 );
//	Vetor* v2 = new Vetor (  0.0,  1.0,  0.0 );
	
//	Vetor* tw = new Vetor;
//	*tw = *v1 * *v2;
	
//	cout << "tw ( " << tw->x << ", " << tw->y << ", " << tw->z << ")" << endl;
	
//	Curva* c1 = new CurvParamHermite ( *p1, *p2, *v1, *v1 );
//	Curva* c2 = new CurvParamHermite ( *p2, *p4, *v2, *v2 );
//	Curva* c3 = new CurvParamHermite ( *p3, *p4, *v1, *v1 );
//	Curva* c4 = new CurvParamHermite ( *p1, *p3, *v2, *v2 );

//	cout	<< "c1->get_L ( ) = " << c1->get_L ( ) << "\n"
//			<< "c2->get_L ( ) = " << c2->get_L ( ) << "\n"
//			<< "c3->get_L ( ) = " << c3->get_L ( ) << "\n"
//			<< "c4->get_L ( ) = " << c4->get_L ( ) << endl;
			
//	cout	<< static_cast < CurvParamHermite* > ( c1 )->getP0 ( ).x << " "
//			<< static_cast < CurvParamHermite* > ( c1 )->getP0 ( ).y << " "
//			<< static_cast < CurvParamHermite* > ( c1 )->getP0 ( ).z << endl;
	
//	Patch* patch = new HermitePatch ( c1, c2, c3, c4, *tw, *tw, *tw, *tw );
	
//	cout << static_cast < CurvParamHermite* > ( c1 )->getNumDePatches ( ) << endl;
	
//	double u = 1;
//	double v = 0.6;
		
//	Ponto pt;
	
//	pt = static_cast < HermitePatch* > ( patch )->parametrizar ( u , v );
	
//	cout << "pt: " << pt.x << ", " << pt.y << ", " << pt.z << endl;

//	tuple < double, double > t = static_cast < HermitePatch* > ( patch )->encontrar_u_v ( pt );
	
//        cout <<  "t: " << get < 0 > ( t ) << ", " << get < 1 > ( t ) << endl;
	
//	cout	<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (0,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (0,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (0,0) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (0,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (0,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (0,1) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (0,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (0,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (0,2) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (0,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (0,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (0,3) << ")"<< endl;
			
//	cout	<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (1,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (1,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (1,0) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (1,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (1,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (1,1) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (1,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (1,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (1,2) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (1,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (1,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (1,3) << ")"<< endl;

//	cout	<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (2,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (2,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (2,0) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (2,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (2,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (2,1) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (2,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (2,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (2,2) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (2,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (2,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (2,3) << ")"<< endl;

//	cout	<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (3,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (3,0) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (3,0) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (3,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (3,1) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (3,1) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (3,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (3,2) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (3,2) << ")\t"
//			<< "( "
//			<< static_cast < HermitePatch* > ( patch )->getGx().getElem (3,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGy().getElem (3,3) << ", "
//			<< static_cast < HermitePatch* > ( patch )->getGz().getElem (3,3) << ")"<< endl;

//	delete static_cast < HermitePatch* > ( patch );
//	delete tw;
//	delete v2;
//	delete v1;
//	delete c4;
//	delete c3;
//	delete c2;
//	delete c1;
//	delete p4;
//	delete p3;
//	delete p2;
//	delete p1;

	
	
//	return 0;
//}
