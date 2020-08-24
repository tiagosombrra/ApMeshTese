#include "Data/Definitions.h"

#include "Data/Point3D.h"
#include "Data/Triangle3D.h"

int main()
{
	//TODO testar, qd os metodos com parametros ponteiros chamam os metodos
	//	com parametros nao ponteiros, se os parametros sao criados, ou apenas
	//	passados

	Point3D *p1 = new Point3D(                0.0,               10.0,   0.0, 1);
	Point3D *p2 = new Point3D(-10.0*cos(M_PI/6.0), -10.0*sin(M_PI/6.0),  0.0, 3);
	Point3D *p3 = new Point3D( 10.0*cos(M_PI/6.0), -10.0*sin(M_PI/6.0),  0.0, 2);

	Triangle3D t(p1, p2, p3);

	double max = 100.0;
	unsigned int lim = 2*(max) + 1;

	for (unsigned int i = 0; i < lim; i++)
	{
		Point3D p;

		p.setZ(max - i);

		std::cout << "z = " << p.getZ() << ", angulo solido = " << t.solidAngle(p) << std::endl;
	}

	return 1;
}
