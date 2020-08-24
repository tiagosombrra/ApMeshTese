#include "Data/Triangle3D.h"

#include "Data/Point2D.h"
#include "Data/Edge2D.h"
#include "Data/Vector3D.h"
#include "Data/Edge.h"
#include "Data/State.h"
#include "Data/Identifiable.h"

using namespace Data;

Data::Triangle3D::Triangle3D(Point3D *p1, Point3D *p2, Point3D *p3) :
	Triangle(p1, p2, p3)
{

}

Data::Triangle3D::Triangle3D(const Point3D &p1, const Point3D &p2, const Point3D &p3) :
	Triangle(NULL, NULL, NULL)
{
	this->setPoints(new Point3D(p1), new Point3D(p2), new Point3D(p3));
}

Data::Triangle3D::~Triangle3D()
{

}

UInt Data::Triangle3D::dimension() const
{
	return 3;
}

bool Data::Triangle3D::in(const Point &p) const
{
	return this->in(&p);
}

bool Data::Triangle3D::in(const Point *p) const
{
	return this->in(static_cast<const Point3D *>(p));
}

bool Data::Triangle3D::in(const Point3D &p) const
{
	return this->in(&p);
}

bool Data::Triangle3D::in(const Point3D *p) const
{
	/*R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);*/

#if USE_C__11
	Real l1, l2, l3;

	std::tie(l1, l2, l3) = this->barycentricCoordinates(p);
#else
	R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
#endif //#if USE_C__11

	return (std::fabs(l1 + l2 + l3 - 1.0) < Data::getTolerance());
}

bool Data::Triangle3D::on(const Point &p) const
{
	return this->on(&p);
}

bool Data::Triangle3D::on(const Point *p) const
{
	return this->on(static_cast<const Point3D *>(p));
}

bool Data::Triangle3D::on(const Point3D &p) const
{
	return this->on(&p);
}

bool Data::Triangle3D::on(const Point3D *p) const
{
	return ((!this->in(p)) && (!this->out(p)));
}

bool Data::Triangle3D::out(const Point &p) const
{
	return this->out(&p);
}

bool Data::Triangle3D::out(const Point *p) const
{
	return this->out(static_cast<const Point3D *>(p));
}

bool Data::Triangle3D::out(const Point3D &p) const
{
	return this->out(&p);
}

bool Data::Triangle3D::out(const Point3D *p) const
{
	/*R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = std::get<0>(t);
	Real l2 = std::get<1>(t);
	Real l3 = std::get<2>(t);*/

#if USE_C__11
	Real l1, l2, l3;

	std::tie(l1, l2, l3) = this->barycentricCoordinates(p);
#else
	R3Tuple t = this->barycentricCoordinates(p);

	Real l1 = t.values[0];
	Real l2 = t.values[1];
	Real l3 = t.values[2];
#endif //#if USE_C__11

	return (std::fabs(l1 + l2 + l3 - 1.0) > Data::getTolerance());
}

bool Data::Triangle3D::intercept(const Triangle3D &t) const
{
	return this->intercept(&t);
}

bool Data::Triangle3D::intercept(const Triangle3D *t) const
{
	//does not test boxes interception, just like the Edge2D::intercept()
	//if (!this->box().intercept(t.box())) return false;

	/*//dumb way
	for (UInt i = 0; i < 3; i++)
	{
		Point *p1 = t->getPoint(i);
		Point *p2 = t->getPoint((i + 1)%3);

		if ((!this->have(p1)) && (!this->have(*p1)) &&
			(!this->have(p2)) && (!this->have(*p2)))
		{
			Edge e(*p1, *p2);

			if (this->intercept(e))
			{
				return true;
			}
		}

		p1 = this->getPoint(i);
		p2 = this->getPoint((i + 1)%3);

		if ((!t->have(p1)) && (!t->have(*p1)) &&
			(!t->have(p2)) && (!t->have(*p2)))
		{
			Edge e(*p1, *p2);

			if (t->intercept(e))
			{
				return true;
			}
		}
	}

	return false;*/

	bool shareOne = false;

	for (UInt i = 0; i < 3; i++)
	{
		Point *p = this->getPoint(i);

		if ((t->have(p)) || (t->have(*p)))
		{
			if (!shareOne)
			{
				shareOne = true;
			}
			else
			{
				return false;
			}
		}
	}

	//std::cout << "shareOne = " << std::boolalpha << shareOne << std::endl;

	//Code based on book Geometric Tools for Computer Graphics
	Vector3D n = this->normal();
	//std::cout << "n = " << n.text() << std::endl;
	//n.normalize();
	//std::cout << "n = " << n.text() << std::endl;

	Vector3D p(this->getP1());
	//std::cout << "p = " << p.text() << std::endl;

	Real d = -n.dot(p);
	//std::cout << "d = " << d << std::endl;

	Real tdists[3];

	for (UInt i = 0; i < 3; i++)
	{
		tdists[i] = Triangle3D::signedPlaneDistance(static_cast<Data::Point3D *>(t->getPoint(i)), &n, d);
		//std::cout << "tdists[" << i << "] = " << tdists[i] << std::endl;
	}

	//if (((tdists[0] < 0.0) && (tdists[1] < 0.0) && (tdists[2] < 0.0)) ||
	//	((tdists[0] > 0.0) && (tdists[1] > 0.0) && (tdists[2] > 0.0)))
	if (((tdists[0] < -Data::getTolerance()) && (tdists[1] < -Data::getTolerance()) && (tdists[2] < -Data::getTolerance())) ||
		((tdists[0] >  Data::getTolerance()) && (tdists[1] >  Data::getTolerance()) && (tdists[2] >  Data::getTolerance())))
	{
		//std::cout << "returning false" << std::endl;
		return false;
	}

	Vector3D tn = t->normal();
	//std::cout << "tn = " << tn.text() << std::endl;
	//tn.normalize();
	//std::cout << "tn = " << tn.text() << std::endl;

	Vector3D tp(t->getP1());
	//std::cout << "tp = " << tp.text() << std::endl;

	Real td = -tn.dot(tp);
	//std::cout << "td = " << td << std::endl;

	Real n1n2 = n.dot(tn);
	//std::cout << "n1n2 = " << n1n2 << std::endl;

	//if both triangles lie in parallel planes
	//if (1.0 - std::fabs(tn.dot(n)) < Data::getTolerance())
	if (1.0 - std::fabs(n1n2) < Data::getTolerance())
	{
		//std::cout << "parallel planes" << std::endl;

		//if the planes are not the same
		if (std::fabs(td - d) > Data::getTolerance())
		{
			return false;
		}

		//project the points of the triangles in the plane (x, y or z) that
		//  is the most parallel to the triangles
		Vector3D u[3];

		for (UInt i = 0; i < 3; i++)
		{
			u[i].setCoord(i, 1.0);
		}

		UInt max = 3;
		Real maxValue = -REAL_MAX;

		for (UInt i = 0; i < 3; i++)
		{
			Real dot = std::fabs(u[i].dot(n));

			if (dot > maxValue)
			{
				maxValue = dot;
				max = i;
			}
		}

		Point2D p[3], tp[3];

		UInt coord = 0;

		for (UInt i = 0; i < 3; i++)
		{
			if (i == max)
			{
				continue;
			}

			for (UInt j = 0; j < 3; j++)
			{
				p[j].setCoord(coord, this->getPoint(j)->getCoord(i));
				tp[j].setCoord(coord, t->getPoint(j)->getCoord(i));
			}

			coord++;
		}

		//create 2D edges for the triangles
		Edge2D e[3], te[3];

		for (UInt i = 0; i < 3; i++)
		{
			e[i].setPoints(&p[i], &p[(i + 1)%3]);
			te[i].setPoints(&tp[i], &tp[(i + 1)%3]);
		}

		//test for 2D edge interceptions
		for (UInt i = 0; i < 3; i++)
		{
			for (UInt j = 0; j < 3; j++)
			{
				if (e[i].intercept(te[j]))
				{
					return true;
				}
			}
		}

		return false;
	}

	Real dists[3];

	for (UInt i = 0; i < 3; i++)
	{
		dists[i] = Triangle3D::signedPlaneDistance(static_cast<Data::Point3D *>(this->getPoint(i)), &tn, td);
		//std::cout << "dists[" << i << "] = " << dists[i] << std::endl;
	}

	//if (((dists[0] < 0.0) && (dists[1] < 0.0) && (dists[2] < 0.0)) ||
	//	((dists[0] > 0.0) && (dists[1] > 0.0) && (dists[2] > 0.0)))
	if (((dists[0] < -Data::getTolerance()) && (dists[1] < -Data::getTolerance()) && (dists[2] < -Data::getTolerance())) ||
		((dists[0] >  Data::getTolerance()) && (dists[1] >  Data::getTolerance()) && (dists[2] >  Data::getTolerance())))
	{
		//std::cout << "returning false" << std::endl;
		return false;
	}

	//line direction and point
	Vector3D ld = n.cross(tn);
	//std::cout << "ld = " << ld.text() << std::endl;

	//Real n1n2 = n.dot(tn);
	Real n1 = 1.0; //n.dot(n);
	//std::cout << "n1 = " << n1 << std::endl;
	Real n2 = 1.0; //tn.dot(tn);
	//std::cout << "n2 = " << n2 << std::endl;

	Real den = n1n2*n1n2 - n1*n2;
	//std::cout << "den = " << den << std::endl;

	d *= -1.0;
	td *= -1.0;

	Real a = (td*n1n2 - d*n2)/(den);
	//std::cout << "a = " << a << std::endl;
	Real b = (d*n1n2 - td*n1)/(den);
	//std::cout << "b = " << b << std::endl;

	Vector3D ln1(n), ln2(tn);
	//std::cout << "ln1 = " << ln1.text() << std::endl;
	//std::cout << "ln2 = " << ln2.text() << std::endl;
	ln1.multiply(a);
	//std::cout << "ln1 = " << ln1.text() << std::endl;
	ln2.multiply(b);
	//std::cout << "ln2 = " << ln2.text() << std::endl;

	Point3D lp(ln1);
	lp.sum(ln2);
	//std::cout << "lp = " << lp.text() << std::endl;

	Real proj[3], tproj[3];

	for (UInt i = 0; i < 3; i++)
	{
		proj[i] = ld.dot(Vector3D(&lp, static_cast<Point3D *>(this->getPoint(i))));
		//std::cout << "proj[" << i << "] = " << proj[i] << std::endl;
		tproj[i] = ld.dot(Vector3D(&lp, static_cast<Point3D *>(t->getPoint(i))));
		//std::cout << "tproj[" << i << "] = " << tproj[i] << std::endl;
	}

	//these are the parameters of the line that is the intersection of a
	//  triangle and the plane of the other triangle
	Real param[2], tparam[2];
	param[0] = param[1] = tparam[0] = tparam[1] = 0.0;

	//this boolean means that at least one of the distances of the points to the
	//  interception line is 0
	bool zero = false, tzero = false;

	for (UInt i = 0; i < 3; i++)
	{
		if (std::fabs(dists[i]) < Data::getTolerance())
		{
			dists[i] = 0.0;

			zero = true;
		}

		if (std::fabs(tdists[i]) < Data::getTolerance())
		{
			tdists[i] = 0.0;

			tzero = true;
		}
	}

	//std::cout << "zero = " << std::boolalpha << zero << std::endl;
	//for the this triangle:

	//if one or two points hit the other plane
	if (zero)
	{
		UInt z1 = 4, z2 = 4;

		for (UInt i = 0; i < 3; i++)
		{
			//if (dists[i] < Data::getTolerance())
			if (dists[i] == 0.0)
			{
				if (z1 == 4)
				{
					z1 = i;
				}
				else
				{
					z2 = i;

					break;
				}
			}
		}

		//std::cout << "z1 = " << z1 << std::endl;
		//std::cout << "z2 = " << z2 << std::endl;

		param[0] = proj[z1];
		//std::cout << "param[0] = " << param[0] << std::endl;

		//if two points hit the other plane
		if (z2 != 4)
		{
			param[1] = proj[z2];
			//std::cout << "param[1] = " << param[1] << std::endl;
		}
		else
		{
			//if there are two points in the same side
			if (((dists[(z1 + 1)%3] < 0.0) && (dists[(z1 + 2)%3] < 0.0)) ||
				((dists[(z1 + 1)%3] > 0.0) && (dists[(z1 + 2)%3] > 0.0)))
			{
				param[1] = proj[z1];
				//std::cout << "param[1] = " << param[1] << std::endl;
			}
			else
			{
				z2 = (z1 + 1)%3;
				//std::cout << "z2 = " << z2 << std::endl;
				z1 = (z1 + 2)%3;
				//std::cout << "z1 = " << z1 << std::endl;

				param[1] = proj[z2] + (proj[z1] - proj[z2])*dists[z2]/(dists[z2] - dists[z1]);
				//std::cout << "param[1] = " << param[1] << std::endl;
			}
		}
	}
	else
	{
		UInt different = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (((dists[i] < 0.0) && (dists[(i+1)%3] > 0.0) && (dists[(i+2)%3] > 0.0)) ||
				((dists[i] > 0.0) && (dists[(i+1)%3] < 0.0) && (dists[(i+2)%3] < 0.0)))
			{
				different = i;

				break;
			}
		}

		//std::cout << "different = " << different << std::endl;

		UInt z2 = (different + 1)%3;
		//std::cout << "z2 = " << z2 << std::endl;
		UInt z1 = (different + 2)%3;
		//std::cout << "z1 = " << z1 << std::endl;

		param[0] = proj[z1] + (proj[different] - proj[z1])*dists[z1]/(dists[z1] - dists[different]);
		//std::cout << "param[0] = " << param[0] << std::endl;
		param[1] = proj[z2] + (proj[different] - proj[z2])*dists[z2]/(dists[z2] - dists[different]);
		//std::cout << "param[1] = " << param[1] << std::endl;
	}

	if (param[0] > param[1])
	{
		Real tmp = param[0];
		param[0] = param[1];
		param[1] = tmp;
	}

	//std::cout << "param[0] = " << param[0] << std::endl;
	//std::cout << "param[1] = " << param[1] << std::endl;

	//std::cout << "tzero = " << std::boolalpha << tzero << std::endl;
	//for the other triangle:

	//if one or two points hit the other plane
	if (tzero)
	{
		UInt z1 = 4, z2 = 4;

		for (UInt i = 0; i < 3; i++)
		{
			//if (tdists[i] < Data::getTolerance())
			if (tdists[i] == 0.0)
			{
				if (z1 == 4)
				{
					z1 = i;
				}
				else
				{
					z2 = i;

					break;
				}
			}
		}

		//std::cout << "z1 = " << z1 << std::endl;
		//std::cout << "z2 = " << z2 << std::endl;

		tparam[0] = tproj[z1];
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;

		//if two points hit the other plane
		if (z2 != 4)
		{
			tparam[1] = tproj[z2];
			//std::cout << "tparam[1] = " << tparam[1] << std::endl;
		}
		else
		{
			//if there are two points in the same side
			if (((tdists[(z1 + 1)%3] < 0.0) && (tdists[(z1 + 2)%3] < 0.0)) ||
				((tdists[(z1 + 1)%3] > 0.0) && (tdists[(z1 + 2)%3] > 0.0)))
			{
				tparam[1] = tproj[z1];
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
			}
			else
			{
				z2 = (z1 + 1)%3;
				//std::cout << "z2 = " << z2 << std::endl;
				z1 = (z1 + 2)%3;
				//std::cout << "z1 = " << z1 << std::endl;

				tparam[1] = tproj[z2] + (tproj[z1] - tproj[z2])*tdists[z2]/(tdists[z2] - tdists[z1]);
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
			}
		}
	}
	else
	{
		UInt different = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (((tdists[i] < 0.0) && (tdists[(i+1)%3] > 0.0) && (tdists[(i+2)%3] > 0.0)) ||
				((tdists[i] > 0.0) && (tdists[(i+1)%3] < 0.0) && (tdists[(i+2)%3] < 0.0)))
			{
				different = i;

				break;
			}
		}

		//std::cout << "different = " << different << std::endl;

		UInt z2 = (different + 1)%3;
		//std::cout << "z2 = " << z2 << std::endl;
		UInt z1 = (different + 2)%3;
		//std::cout << "z1 = " << z1 << std::endl;

		tparam[0] = tproj[z1] + (tproj[different] - tproj[z1])*tdists[z1]/(tdists[z1] - tdists[different]);
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;
		tparam[1] = tproj[z2] + (tproj[different] - tproj[z2])*tdists[z2]/(tdists[z2] - tdists[different]);
		//std::cout << "tparam[1] = " << tparam[1] << std::endl;
	}

	if (tparam[0] > tparam[1])
	{
		Real tmp = tparam[0];
		tparam[0] = tparam[1];
		tparam[1] = tmp;
	}

	//std::cout << "tparam[0] = " << tparam[0] << std::endl;
	//std::cout << "tparam[1] = " << tparam[1] << std::endl;

	if ((shareOne) &&
		((((param[0] == tparam[0]) || (param[1] == tparam[1])) &&
		  ((param[0] == param[1]) || (tparam[0] == tparam[1]))) ||
		 (param[1] == tparam[0]) ||
		 (param[0] == tparam[1])))
		/*(param[0] == param[1]) &&
		(tparam[0] == tparam[1]) &&
		(param[0] == tparam[0]))*/
		/*(std::fabs(param[0] - param[1]) < Data::getTolerance()) &&
		(std::fabs(tparam[0] - tparam[1]) < Data::getTolerance()) &&
		(std::fabs(param[0] - tparam[0]) < Data::getTolerance()))*/
	{
		//std::cout << "return false 1" << std::endl;
		return false;
	}

	if ((param[1] < tparam[0] + Data::getTolerance()) ||
		(tparam[1] < param[0] + Data::getTolerance()))
	{
		//std::cout << "return false 2" << std::endl;
		return false;
	}

	//std::cout << "return true" << std::endl;
	return true;
}

bool Data::Triangle3D::intercept(const Edge &e) const
{
	return this->intercept(&e);
}

bool Data::Triangle3D::intercept(const Edge *e) const
{
	//Code based on book Geometric Tools for Computer Graphics
	Vector3D d(e->getP1(), e->getP2());
	Vector3D e1(this->getP1(), this->getP2());
	Vector3D e2(this->getP1(), this->getP3());

	Real dnorm = d.norm();
	d.multiply(1.0/dnorm);

	Vector3D p = d.cross(e2);

	Real m = p.dot(e1);

	if (std::fabs(m) < Data::getTolerance())
	{
		return false;
	}

	m = 1.0/m;

	Vector3D s(this->getP1(), e->getP1());

	Real u = m*s.dot(p);

	if ((u < Data::getTolerance()) || (u > (1.0 - Data::getTolerance())))
	//if ((u < 0.0) || (u > 1.0))
	{
		return false;
	}

	Vector3D q = s.cross(e1);

	Real v = m*d.dot(q);

	if ((v < Data::getTolerance()) || (v > (1.0 - Data::getTolerance())))
	//if ((v < 0.0) || (v > 1.0))
	{
		return false;
	}

	if ((u + v) > (1.0 - Data::getTolerance()))
	//if (u + v > 1.0)
	{
		return false;
	}

	Real t = m*e2.dot(q);

	if ((t < Data::getTolerance()) || (t > dnorm - Data::getTolerance()))
	//if ((t < Data::getTolerance()) || (t > 1.0 - Data::getTolerance()))
	//if ((t < 0.0) || (t > 1.0))
	{
		return false;
	}

	return true;
}

bool Data::Triangle3D::intercept(const Point3D &p1, const Point3D &p2, const Point3D &p3) const
{
	return this->intercept(&p1, &p2, &p3);
}

bool Data::Triangle3D::intercept(const Point3D *p1, const Point3D *p2, const Point3D *p3) const
{
	Triangle3D t(p1, p2, p3);

	bool intercept = this->intercept(t);

	t.setPoints(NULL, NULL, NULL);

	return intercept;
}

#if USE_EXACT
bool Data::Triangle3D::eintercept(const Triangle3D &t) const
{
	return this->eintercept(&t);
}

bool Data::Triangle3D::eintercept(const Triangle3D *t) const
{
	//does not test boxes interception, just like the Edge2D::intercept()
	//if (!this->box().intercept(t.box())) return false;

	/*//dumb way
	for (UInt i = 0; i < 3; i++)
	{
		Point *p1 = t->getPoint(i);
		Point *p2 = t->getPoint((i + 1)%3);

		if ((!this->have(p1)) && (!this->have(*p1)) &&
			(!this->have(p2)) && (!this->have(*p2)))
		{
			Edge e(*p1, *p2);

			if (this->eintercept(e))
			{
				return true;
			}
		}

		p1 = this->getPoint(i);
		p2 = this->getPoint((i + 1)%3);

		if ((!t->have(p1)) && (!t->have(*p1)) &&
			(!t->have(p2)) && (!t->have(*p2)))
		{
			Edge e(*p1, *p2);

			if (t->eintercept(e))
			{
				return true;
			}
		}
	}

	return false;*/

	bool shareOne = false, shareTwo = false;

	for (UInt i = 0; i < 3; i++)
	{
		Point *p = this->getPoint(i);

		bool match = false;

		for (UInt j = 0; j < 3; j++)
		{
			if ((t->getPoint(j) == p) ||
				(t->getPoint(j)->match(p)))
			{
				match = true;

				break;
			}
		}

		if (match)
		{
			if (!shareOne)
			{
				shareOne = true;
			}
			else if (!shareTwo)
			{
				//return false;
				shareTwo = true;
				//break;
			}
			else
			{
				//shareThree = true;
				return false;
			}
		}
	}

	//std::cout << "shareOne = " << std::boolalpha << shareOne << std::endl;

	//Code based on book Geometric Tools for Computer Graphics
	EVector3D n = this->enormal();
	//n.normalize();
	//std::cout << "n = " << n.text() << std::endl;
	//std::cout << "n.norm() = " << n.norm() << std::endl;

	EVector3D p(static_cast<Point3D *>(this->getP1())->epoint3D());
	//std::cout << "p = " << p.text() << std::endl;

	EReal d = -n.dot(p);
	//std::cout << "d = " << d << std::endl;

	EReal tdists[3];

	if (!shareTwo)
	{
		for (UInt i = 0; i < 3; i++)
		{
			tdists[i] = Triangle3D::esignedPlaneDistance(static_cast<Data::Point3D *>(t->getPoint(i))->epoint3D(), n, d);
			//std::cout << "tdists[" << i << "] = " << tdists[i] << std::endl;
		}

		if (((tdists[0] < 0.0) && (tdists[1] < 0.0) && (tdists[2] < 0.0)) ||
			((tdists[0] > 0.0) && (tdists[1] > 0.0) && (tdists[2] > 0.0)))
		{
			//std::cout << "returning false" << std::endl;
			return false;
		}
	}

	EVector3D tn = t->enormal();
	//tn.normalize();
	//std::cout << "tn = " << tn.text() << std::endl;
	//std::cout << "tn.norm() = " << tn.norm() << std::endl;

	EVector3D tp(static_cast<Point3D *>(t->getP1())->epoint3D());
	//std::cout << "tp = " << tp.text() << std::endl;

	EReal td = -tn.dot(tp);
	//std::cout << "td = " << td << std::endl;

	EReal n1n2 = n.dot(tn);
	//std::cout << "n1n2 = " << n1n2 << std::endl;
	EReal n1 = n.dot(n);
	//std::cout << "n1 = " << n1 << std::endl;
	EReal n2 = tn.dot(tn);
	//std::cout << "n2 = " << n2 << std::endl;
	EReal n1n22 = n1n2*n1n2;
	//std::cout << "n1n22 = " << n1n22 << std::endl;
	EReal den = (n1n22 - n1*n2);
	//std::cout << "den = " << den << std::endl;

	if ((shareTwo) && (den != 0.0))
	{
		return false;
	}

	//if both triangles lie in parallel planes
	//if (1.0 - std::fabs(tn.dot(n)) < Data::getTolerance())
	//if (n1n2 == 1.0)
	//if (n1n22 == n1*n2)
	if ((shareTwo) || (den == 0.0))
	{
		//std::cout << "parallel planes" << std::endl;

		//if the planes are not the same
		if (td != d)
		{
			return false;
		}

		/*//project the points of the triangles in the plane (x, y or z) that
		//  is the most parallel to the triangles
		EVector3D u[3];

		for (UInt i = 0; i < 3; i++)
		{
			u[i].setCoord(i, 1.0);
		}*/

		UInt max = 0;
		EReal maxValue = std::fabs(n.getCoord(0));

		for (UInt i = 1; i < 3; i++)
		{
			//EReal dot = std::fabs(u[i].dot(n));
			EReal val = std::fabs(n.getCoord(i));

			//if (dot > maxValue)
			if (val > maxValue)
			{
				//maxValue = dot;
				maxValue = val;
				max = i;
			}
		}

		if (shareTwo)
		{
			UInt thisp1 = 4, thisp2 = 4, tp1 = 4, tp2 = 4;

			for (UInt i = 0; i < 3; i++)
			{
				for (UInt j = 0; j < 3; j++)
				{
					if ((this->getPoint(i) == t->getPoint(j)) ||
						(this->getPoint(i)->match(t->getPoint(j))))
					{
						if (thisp1 == 4)
						{
							thisp1 = i;
							tp1 = j;
						}
						else if (thisp2 == 4)
						{
							thisp2 = i;
							tp2 = j;
						}
						else
						{
							//all three points are equal
							return false;
						}

						break;
					}
				}
			}

			UInt o1 = 4, o2 = 4;

			for (UInt i = 0; i < 3; i++)
			{
				if ((o1 == 4) && (i != thisp1) && (i != thisp2))
				{
					o1 = i;
				}

				if ((o2 == 4) && (i != tp1) && (i != tp2))
				{
					o2 = i;
				}

				if ((o1 != 4) && (o2 != 4))
				{
					break;
				}
			}

			EPoint2D ep1, ep2, p1, p2;

			for (UInt i = 0, coord = 0; i < 3; i++)
			{
				if (i == max)
				{
					continue;
				}

				ep1.setCoord(coord, this->getPoint(thisp1)->getCoord(i));
				ep2.setCoord(coord, this->getPoint(thisp2)->getCoord(i));

				p1.setCoord(coord, this->getPoint(o1)->getCoord(i));
				p2.setCoord(coord, t->getPoint(o2)->getCoord(i));

				coord++;
			}

			EVector2D v(ep1, ep2), v1(ep1, p1), v2(ep1, p2);

			EReal s1 = v1.cross(v);
			EReal s2 = v2.cross(v);

			if (((s1 < 0.0) && (s2 > 0.0)) ||
				((s1 > 0.0) && (s2 < 0.0)))
			{
				return false;
			}

			return true;
		}

		Point2D p[3], tp[3];

		UInt coord = 0;

		for (UInt i = 0; i < 3; i++)
		{
			if (i == max)
			{
				continue;
			}

			for (UInt j = 0; j < 3; j++)
			{
				p[j].setCoord(coord, this->getPoint(j)->getCoord(i));
				tp[j].setCoord(coord, t->getPoint(j)->getCoord(i));
			}

			coord++;
		}

		//create 2D edges for the triangles
		Edge2D e[3], te[3];

		for (UInt i = 0; i < 3; i++)
		{
			e[i].setPoints(&p[i], &p[(i + 1)%3]);
			te[i].setPoints(&tp[i], &tp[(i + 1)%3]);
		}

		//test for 2D edge interceptions
		for (UInt i = 0; i < 3; i++)
		{
			for (UInt j = 0; j < 3; j++)
			{
				if (e[i].eintercept(te[j]))
				{
					return true;
				}
			}
		}

		return false;
	}

	EReal dists[3];

	for (UInt i = 0; i < 3; i++)
	{
		dists[i] = Triangle3D::esignedPlaneDistance(static_cast<Data::Point3D *>(this->getPoint(i))->epoint3D(), tn, td);
		//std::cout << "dists[" << i << "] = " << dists[i] << std::endl;
	}

	if (((dists[0] < 0.0) && (dists[1] < 0.0) && (dists[2] < 0.0)) ||
		((dists[0] > 0.0) && (dists[1] > 0.0) && (dists[2] > 0.0)))
	{
		//std::cout << "returning false";
		return false;
	}

	//line direction and point
	EVector3D ld = n.cross(tn);
	//std::cout << "ld = " << ld.text() << std::endl;

	EReal a = -td*n1n2 + d*n2;
	//std::cout << "a = " << a << std::endl;
	EReal b = -d*n1n2 + td*n1;
	//std::cout << "b = " << b << std::endl;

	EVector3D ln1(n), ln2(tn);
	//std::cout << "ln1 = " << ln1.text() << std::endl;
	//std::cout << "ln2 = " << ln2.text() << std::endl;
	ln1.multiply(a);
	//std::cout << "ln1 = " << ln1.text() << std::endl;
	ln2.multiply(b);
	//std::cout << "ln2 = " << ln2.text() << std::endl;

	EPoint3D lp(ln1);
	lp.sum(ln2);
	//std::cout << "lp = " << lp.text() << std::endl;

	EReal proj[3], tproj[3];

	for (UInt i = 0; i < 3; i++)
	{
		proj[i] = ld.dot(EVector3D(lp, static_cast<Point3D *>(this->getPoint(i))->epoint3D()));
		//std::cout << "proj[" << i << "] = " << proj[i] << std::endl;
		tproj[i] = ld.dot(EVector3D(lp, static_cast<Point3D *>(t->getPoint(i))->epoint3D()));
		//std::cout << "tproj[" << i << "] = " << tproj[i] << std::endl;
	}

	//these are the parameters of the line that is the intersection of a
	//  triangle and the plane of the other triangle
	EFReal param[2], tparam[2];

	//this boolean means that at least one of the distances of the points to the
	//  interception line is 0
	bool zero = false, tzero = false;

	for (UInt i = 0; i < 3; i++)
	{
		dists[i].compress();
		tdists[i].compress();

		if (dists[i] == 0.0)
		{
			zero = true;
		}

		if (tdists[i] == 0.0)
		{
			tzero = true;
		}
	}

	//std::cout << "zero = " << std::boolalpha << zero << std::endl;
	//for the this triangle:

	//if one or two points hit the other plane
	if (zero)
	{
		UInt z1 = 4, z2 = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (dists[i] == 0.0)
			{
				if (z1 == 4)
				{
					z1 = i;
				}
				else
				{
					z2 = i;

					break;
				}
			}
		}

		//std::cout << "z1 = " << z1 << std::endl;
		//std::cout << "z2 = " << z2 << std::endl;

		param[0] = proj[z1];
		//std::cout << "param[0] = " << param[0] << std::endl;

		//if two points hit the other plane
		if (z2 != 4)
		{
			param[1] = proj[z2];
			//std::cout << "param[1] = " << param[1] << std::endl;
		}
		else
		{
			//if there are two points in the same side
			if (((dists[(z1 + 1)%3] < 0.0) && (dists[(z1 + 2)%3] < 0.0)) ||
				((dists[(z1 + 1)%3] > 0.0) && (dists[(z1 + 2)%3] > 0.0)))
			{
				param[1] = proj[z1];
				//std::cout << "param[1] = " << param[1] << std::endl;
			}
			else
			{
				z2 = (z1 + 1)%3;
				//std::cout << "z2 = " << z2 << std::endl;
				z1 = (z1 + 2)%3;
				//std::cout << "z1 = " << z1 << std::endl;

				//param[1] = proj[z2] + (proj[z1] - proj[z2])*dists[z2]/(dists[z2] - dists[z1]);
				param[1] = (proj[z1] - proj[z2])*dists[z2];
				//std::cout << "param[1] = " << param[1] << std::endl;
				param[1] /= dists[z2] - dists[z1];
				//std::cout << "param[1] = " << param[1] << std::endl;
				param[1] += proj[z2];
				//std::cout << "param[1] = " << param[1] << std::endl;
			}
		}
	}
	else
	{
		UInt different = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (((dists[i] < 0.0) && (dists[(i+1)%3] > 0.0) && (dists[(i+2)%3] > 0.0)) ||
				((dists[i] > 0.0) && (dists[(i+1)%3] < 0.0) && (dists[(i+2)%3] < 0.0)))
			{
				different = i;

				break;
			}
		}

		//std::cout << "different = " << different << std::endl;

		UInt z2 = (different + 1)%3;
		//std::cout << "z2 = " << z2 << std::endl;
		UInt z1 = (different + 2)%3;
		//std::cout << "z1 = " << z1 << std::endl;

		//param[0] = proj[z1] + (proj[different] - proj[z1])*dists[z1]/(dists[z1] - dists[different]);
		param[0] = (proj[different] - proj[z1])*dists[z1];
		//std::cout << "param[0] = " << param[0] << std::endl;
		param[0] /= dists[z1] - dists[different];
		//std::cout << "param[0] = " << param[0] << std::endl;
		param[0] += proj[z1];
		//std::cout << "param[0] = " << param[0] << std::endl;
		//param[1] = proj[z2] + (proj[different] - proj[z2])*dists[z2]/(dists[z2] - dists[different]);
		param[1] = (proj[different] - proj[z2])*dists[z2];
		//std::cout << "param[1] = " << param[1] << std::endl;
		param[1] /= dists[z2] - dists[different];
		//std::cout << "param[1] = " << param[1] << std::endl;
		param[1] += proj[z2];
		//std::cout << "param[1] = " << param[1] << std::endl;
	}

	if (param[0] > param[1])
	{
		param[0].swap(param[1]);
	}

	//param[0] /= den;
	//param[1] /= den;

	//std::cout << "param[0] = " << param[0] << std::endl;
	//std::cout << "param[1] = " << param[1] << std::endl;

	//std::cout << "tzero = " << std::boolalpha << tzero << std::endl;
	//for the other triangle:

	//if one or two points hit the other plane
	if (tzero)
	{
		UInt z1 = 4, z2 = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (tdists[i] == 0.0)
			{
				if (z1 == 4)
				{
					z1 = i;
				}
				else
				{
					z2 = i;

					break;
				}
			}
		}

		//std::cout << "z1 = " << z1 << std::endl;
		//std::cout << "z2 = " << z2 << std::endl;

		tparam[0] = tproj[z1];
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;

		//if two points hit the other plane
		if (z2 != 4)
		{
			tparam[1] = tproj[z2];
			//std::cout << "tparam[1] = " << tparam[1] << std::endl;
		}
		else
		{
			//if there are two points in the same side
			if (((tdists[(z1 + 1)%3] < 0.0) && (tdists[(z1 + 2)%3] < 0.0)) ||
				((tdists[(z1 + 1)%3] > 0.0) && (tdists[(z1 + 2)%3] > 0.0)))
			{
				tparam[1] = tproj[z1];
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
			}
			else
			{
				z2 = (z1 + 1)%3;
				//std::cout << "z2 = " << z2 << std::endl;
				z1 = (z1 + 2)%3;
				//std::cout << "z1 = " << z1 << std::endl;

				//tparam[1] = tproj[z2] + (tproj[z1] - tproj[z2])*tdists[z2]/(tdists[z2] - tdists[z1]);
				tparam[1] = (tproj[z1] - tproj[z2])*tdists[z2];
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
				tparam[1] /= tdists[z2] - tdists[z1];
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
				tparam[1] += tproj[z2];
				//std::cout << "tparam[1] = " << tparam[1] << std::endl;
			}
		}
	}
	else
	{
		UInt different = 4;

		for (UInt i = 0; i < 3; i++)
		{
			if (((tdists[i] < 0.0) && (tdists[(i+1)%3] > 0.0) && (tdists[(i+2)%3] > 0.0)) ||
				((tdists[i] > 0.0) && (tdists[(i+1)%3] < 0.0) && (tdists[(i+2)%3] < 0.0)))
			{
				different = i;

				break;
			}
		}

		//std::cout << "different = " << different << std::endl;

		UInt z2 = (different + 1)%3;
		//std::cout << "z2 = " << z2 << std::endl;
		UInt z1 = (different + 2)%3;
		//std::cout << "z1 = " << z1 << std::endl;

		//tparam[0] = tproj[z1] + (tproj[different] - tproj[z1])*tdists[z1]/(tdists[z1] - tdists[different]);
		tparam[0] = (tproj[different] - tproj[z1])*tdists[z1];
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;
		tparam[0] /= tdists[z1] - tdists[different];
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;
		tparam[0] += tproj[z1];
		//std::cout << "tparam[0] = " << tparam[0] << std::endl;
		//tparam[1] = tproj[z2] + (tproj[different] - tproj[z2])*tdists[z2]/(tdists[z2] - tdists[different]);
		tparam[1] = (tproj[different] - tproj[z2])*tdists[z2];
		//std::cout << "tparam[1] = " << tparam[1] << std::endl;
		tparam[1] /= tdists[z2] - tdists[different];
		//std::cout << "tparam[1] = " << tparam[1] << std::endl;
		tparam[1] += tproj[z2];
		//std::cout << "tparam[1] = " << tparam[1] << std::endl;
	}

	if (tparam[0] > tparam[1])
	{
		tparam[0].swap(tparam[1]);
	}

	//tparam[0] /= den;
	//tparam[1] /= den;

	//std::cout << "tparam[0] = " << tparam[0] << std::endl;
	//std::cout << "tparam[1] = " << tparam[1] << std::endl;

	/*bool p0p1   = (param[0] == param[1]);
	bool tp0tp1 = (tparam[0] == tparam[1]);
	bool p0tp0  = (param[0] == tparam[0]);*/

	if ((shareOne) &&
		/*(((p0p1) && (tp0tp1) && (p0tp0)) ||
		 ((p0p1) &&             (p0tp0)) ||
		 //((p0p1) &&             (param[0] == tparam[1])) ||
		 (          (tp0tp1) && (p0tp0)) ||
		 //(          (tp0tp1) && (param[1] == tparam[0])) ||
		 (                      (param[1] == tparam[0])) ||
		 (                      (param[0] == tparam[1]))
		)*/
		((((param[0] == tparam[0]) || (param[1] == tparam[1])) &&
		  ((param[0] == param[1]) || (tparam[0] == tparam[1]))) ||
		 (param[1] == tparam[0]) ||
		 (param[0] == tparam[1])
		)
	   )
	{
		//std::cout << "return false 1" << std::endl;
		return false;
	}

	if ((param[1] < tparam[0]) ||
		(tparam[1] < param[0]))
	{
		//std::cout << "return false 2" << std::endl;
		return false;
	}

	//std::cout << "return true" << std::endl;
	return true;
}

bool Data::Triangle3D::eintercept(const Edge &e) const
{
	return this->eintercept(&e);
}

bool Data::Triangle3D::eintercept(const Edge *e) const
{
	//Code based on book Geometric Tools for Computer Graphics
	EVector3D d(static_cast<Point3D *>(e->getP1())->epoint3D(), static_cast<Point3D *>(e->getP2())->epoint3D());
	EVector3D e1(static_cast<Point3D *>(this->getP1())->epoint3D(), static_cast<Point3D *>(this->getP2())->epoint3D());
	EVector3D e2(static_cast<Point3D *>(this->getP1())->epoint3D(), static_cast<Point3D *>(this->getP3())->epoint3D());

	EReal dnorm = d.norm();
	d.multiply(1.0/dnorm);

	EVector3D p = d.cross(e2);

	EReal m = p.dot(e1);

	if (m == 0.0)
	{
		return false;
	}

	m = 1.0/m;

	EVector3D s(static_cast<Point3D *>(this->getP1())->epoint3D(), static_cast<Point3D *>(e->getP1())->epoint3D());

	EReal u = m*s.dot(p);

	if ((u < 0.0) || (u > 1.0))
	{
		return false;
	}

	EVector3D q = s.cross(e1);

	EReal v = m*d.dot(q);

	if ((v < 0.0) || (v > 1.0))
	{
		return false;
	}

	if ((u + v) > 1.0)
	{
		return false;
	}

	EReal t = m*e2.dot(q);

	if ((t < 0.0) || (t > dnorm))
	//if ((t < Data::getTolerance()) || (t > 1.0 - Data::getTolerance()))
	//if ((t < 0.0) || (t > 1.0))
	{
		return false;
	}

	return true;
}

bool Data::Triangle3D::eintercept(const Point3D &p1, const Point3D &p2, const Point3D &p3) const
{
	return this->eintercept(&p1, &p2, &p3);
}

bool Data::Triangle3D::eintercept(const Point3D *p1, const Point3D *p2, const Point3D *p3) const
{
	Triangle3D t(p1, p2, p3);

	bool intercept = this->eintercept(t);

	t.setPoints(NULL, NULL, NULL);

	return intercept;
}
#endif //#if USE_EXACT

Point3D Data::Triangle3D::project(const Point3D &p) const
{
	return this->project(&p);
}

Point3D Data::Triangle3D::project(const Point3D *p) const
{
	/*Vector3D n = this->normal();

	//Vector3D q(this->getP1());
	Vector3D q(p, this->getP1());

	Vector3D pproj(p);

	//Real t = -q.dot(p)/q.dot(n);
	Real t = n.dot(q)/n.dot(n);

	n.multiply(t);

	pproj.sum(n);

	return Point3D(pproj);*/

	Vector3D n = this->normal();
	Vector3D q(p, this->getP1());

	//n.normalize();
	n.multiply(q.norm());
	n.sum(p);

	return Point3D(n);
}

/*Real Data::Triangle3D::squaredDistance(const Point3D &p) const
{
	return this->squaredDistance(&p);
}

Real Data::Triangle3D::squaredDistance(const Point3D *p) const
{
	//Code based on book Geometric Tools for Computer Graphics
	Vector3D e0(this->getP2(), this->getP1());
	Vector3D e1(this->getP3(), this->getP1());
	Vector3D bp(this->getP1(), p);

	Real a = e0.dot(e0);
	Real b = e0.dot(e1);
	Real c = e1.dot(e1);
	Real d = e0.dot(bp);
	Real e = e1.dot(bp);
	//Real f = bp.dot(bp);

	Real det = std::fabs(a*c - b*b);
	Real s = (b*e - c*d);
	Real t = (b*d - a*e);
	//Real dist = 0.0;

	if (s + t <= det)
	{
		if (s < 0.0)
		{
			if (t < 0.0)
			{
				//Region 4
				if (d < 0.0)
				{
					t = 0.0;
					s = (-d >= a) ? 1.0 : -d/a;
				}
				else
				{
					s = 0.0;
					t = (e >= 0.0) ? 0.0 : ((-e >= c) ? 1.0 : -e/c);
				}
			}
			else
			{
				//Region 3
				s = 0.0;
				t = (e >= 0.0) ? 0.0 : ((-e >= c) ? 1.0 : -e/c);
			}
		}
		else if (t < 0.0)
		{
			//Region 5
			s = (d > 0.0) ? 0.0 : ((-d >= a) ? 1.0 : -d/a);
			t = 0.0;
		}
		else
		{
			//Region 0
			Real invDet = 1.0/det;

			s *= invDet;
			t *= invDet;
		}
	}
	else
	{
		if (s < 0.0)
		{
			//Region 2
			Real tmp0 = b + d;
			Real tmp1 = c + e;

			if (tmp1 > tmp0)
			{
				Real numer = tmp1 - tmp0;
				Real denom = a - 2.0*b + c;

				s = (numer > denom) ? 1.0 : numer/denom;
				t = 1.0 - s;
			}
			else
			{
				s = 0.0;
				t = (tmp1 <= 0.0) ? 1.0 : ((e >= 0.0) ? 0.0 : -e/c);
			}
		}
		else if (t < 0.0)
		{
			//Region 6
			Real tmp0 = b + e;
			Real tmp1 = a + d;

			if (tmp1 > tmp0)
			{
				Real numer = tmp1 - tmp0;
				Real denom = a - 2.0*b + c;

				t = (numer > denom) ? 1.0 : numer/denom;
				s = 1.0 - t;
			}
			else
			{
				t = 0.0;
				s = (tmp1 <= 0.0) ? 1.0 : ((d >= 0.0) ? 0.0 : -d/a);
			}
		}
		else
		{
			//Region 1
			Real numer = c + e - b - d;

			if (numer <= 0.0)
			{
				s = 0.0;
			}
			else
			{
				Real denom = a - 2.0*b + c;

				s = (numer >= denom) ? 1.0 : numer/denom;
			}

			t = 1.0 - s;
		}
	}

	Point3D q(this->getP1());

	e0.multiply(s);
	q.sum(e0);

	e1.multiply(t);
	q.sum(e1);

	Vector qp(&q, p);

	return qp.dot(qp);

	//for round-off errors
	//return (dist < Data::getTolerance()) ? 0.0 : dist;
}

Real Data::Triangle3D::squaredDistance(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->squaredDistance(p);
}*/

Real Data::Triangle3D::signedPlaneDistance(const Point3D &p, const Vector3D &n, Real d)
{
	return Data::Triangle3D::signedPlaneDistance(&p, &n, d);
}

Real Data::Triangle3D::signedPlaneDistance(const Point3D *p, const Vector3D *n, Real d)
{
#if USE_EXPERIMENTAL_3
	return n->dot(Vector3D(p)) + d;
#else
	return n->dot(p) + d;
#endif //#if USE_EXPERIMENTAL_3
}

Real Data::Triangle3D::signedPlaneDistance(const Point3D &p) const
{
	return this->signedPlaneDistance(&p);
}

Real Data::Triangle3D::signedPlaneDistance(const Point3D *p) const
{
	Vector3D n = this->normal();

	//n.normalize();

#if USE_EXPERIMENTAL_3
	Real d = -n.dot(Vector3D(this->getP1()));
#else
	Real d = -n.dot(this->getP1());
#endif //#if USE_EXPERIMENTAL_3

	return this->signedPlaneDistance(p, &n, d);
}

Real Data::Triangle3D::signedPlaneDistance(Real x, Real y, Real z) const
{
	return this->signedPlaneDistance(Point3D(x, y, z));
}

#if USE_EXACT
EReal Data::Triangle3D::esignedPlaneDistance(const EPoint3D &p, const EVector3D &n, EReal d)
{
	return Data::Triangle3D::esignedPlaneDistance(&p, &n, d);
}

EReal Data::Triangle3D::esignedPlaneDistance(const EPoint3D *p, const EVector3D *n, EReal d)
{
#if USE_EXPERIMENTAL_3
	return n->dot(EVector3D(p)) + d;
#else
	return n->dot(p) + d;
#endif //#if USE_EXPERIMENTAL_3
}

EReal Data::Triangle3D::esignedPlaneDistance(const EPoint3D &p) const
{
	return this->esignedPlaneDistance(&p);
}

EReal Data::Triangle3D::esignedPlaneDistance(const EPoint3D *p) const
{
	EVector3D n = this->enormal();

	n.normalize();

#if USE_EXPERIMENTAL_3
	EReal d = -n.dot(EVector3D(static_cast<Point3D *>(this->getP1())->epoint3D()));
#else
	EReal d = -n.dot(static_cast<Point3D *>(this->getP1())->epoint3D());
#endif //#if USE_EXPERIMENTAL_3

	return this->esignedPlaneDistance(p, &n, d);
}

EReal Data::Triangle3D::esignedPlaneDistance(EReal x, EReal y, EReal z) const
{
	return this->esignedPlaneDistance(EPoint3D(x, y, z));
}
#endif //#if USE_EXACT

Real Data::Triangle3D::straightDistanceEdge(Point3D *p1, Point3D *p2, const Point3D &p, Point2D *p12D, Point2D *p22D, const Point2D &p2D) const
{
	Edge2D e(p12D, p22D);

	Vector2D v(*e.getP1(), p2D);

	Real l = e.length();
#if USE_EXPERIMENTAL_3
	Real vcos = e.vector2D().dot(v)/l;
#else
	Real vcos = e.vector().dot(v)/l;
#endif //#if USE_EXPERIMENTAL_3

	e.setPoints(NULL, NULL);

	if (vcos < Data::getTolerance())
	{
		return p1->distance(p);
	}

	if (vcos > l - Data::getTolerance())
	{
		return p2->distance(p);
	}

	Triangle3D triangle(p1, p2, const_cast<Point3D *>(&p));

	Real dist = 2.0*triangle.surface()/p1->distance(p2);

	triangle.setPoints(NULL, NULL, NULL);

	return dist;
}

Real Data::Triangle3D::straightDistanceEdge(const Point3D *p, UInt p1, UInt p2) const
{
	Vector3D d(static_cast<Point3D *>(this->getPoint(p1)), static_cast<Point3D *>(this->getPoint(p2)));
	Vector3D q(static_cast<Point3D *>(this->getPoint(p1)), p);

	Real t = q.dot(d)/d.dot(d);

	if (t < Data::getTolerance())
	{
		return this->getPoint(p1)->distance(p);
	}

	if (t > 1.0 - Data::getTolerance())
	{
		return this->getPoint(p2)->distance(p);
	}

	d.multiply(t);
	d.sum(this->getPoint(p1));

	return p->distance(Point3D(d));
}

Real Data::Triangle3D::distance(const Point3D &p) const
{
	return this->distance(&p);
}

Real Data::Triangle3D::distance(const Point3D *p) const
{
	//return std::sqrt(this->squaredDistance(p));

	//std::cout << "triangle is " << this->getP1()->text() << "; " << this->getP2()->text() << "; " << this->getP3()->text() << std::endl;
	//std::cout << "p is " << p.text() << std::endl;

	for (UInt i = 0; i < this->numPoints(); i++)
	{
		if (this->getPoint(i)->equal(p))
		{
			return 0.0;
		}
	}

	Point3D projection = this->project(p);

	//std::cout << "projection is " << projection.text() << std::endl;

	/*Real **change;

	change = new Real*[4];

	for (UInt i = 0; i < 4; i++)
	{
		change[i] = new Real[4];
	}

	Triangle2D t = this->convert(NULL, change);

	//std::cout << "t is " << t.getP1()->text() << "; " << t.getP2()->text() << "; " << t.getP3()->text() << std::endl;

	projection.transform(change);

	for (UInt i = 0; i < 4; i++)
	{
		delete [] change[i];
	}

	delete [] change;

	if (std::fabs(projection.getZ()) > Data::getTolerance())
	{
		std::cout << "something is wrong in Real Data::Triangle3D::distance(const Point3D &p) const" << std::endl;

		return 0.0;
	}*/

	Real s = this->orientedSurface();
	Real l1 = Triangle3D(&projection, static_cast<Point3D *>(this->getP2()), static_cast<Point3D *>(this->getP3())).orientedSurface()/s;
	Real l2 = Triangle3D(static_cast<Point3D *>(this->getP1()), &projection, static_cast<Point3D *>(this->getP3())).orientedSurface()/s;
	Real l3 = Triangle3D(static_cast<Point3D *>(this->getP1()), static_cast<Point3D *>(this->getP2()), &projection).orientedSurface()/s;

	/*Point2D pt(projection);

	//std::cout << "pt is " << pt.text() << std::endl;

	R3Tuple barCoords = t.barycentricCoordinates(pt);

	Real l1 = std::get<0>(barCoords);
	Real l2 = std::get<1>(barCoords);
	Real l3 = std::get<2>(barCoords);*/

	//std::cout << "barycentric coords are " << l1 << ", "  << l2 << ", " << l3 << std::endl;

	if ((l1 > Data::getTolerance()) &&
		(l2 > Data::getTolerance()) &&
		(l3 > Data::getTolerance()))
	{
		//std::cout << "distance is " << this->straightDistance(p) << std::endl;

		//return this->straightDistance(p);
		return projection.distance(p);
	}
	else if (l1 <= Data::getTolerance())
	{
		if (l2 <= Data::getTolerance())
		{
			return this->getP3()->distance(p);
		}

		if (l3 <= Data::getTolerance())
		{
			return this->getP2()->distance(p);
		}

		return this->straightDistanceEdge(p, 1, 2);
		/*return this->straightDistanceEdge(
			static_cast<Point3D *>(this->getP2()), static_cast<Point3D *>(this->getP3()), p,
			static_cast<Point2D *>(t.getP2()), static_cast<Point2D *>(t.getP3()), pt);*/
	}
	else if (l2 <= Data::getTolerance())
	{
		if (l3 <= Data::getTolerance())
		{
			return this->getP1()->distance(p);
		}

		return this->straightDistanceEdge(p, 2, 0);
		/*return this->straightDistanceEdge(
			static_cast<Point3D *>(this->getP3()), static_cast<Point3D *>(this->getP1()), p,
			static_cast<Point2D *>(t.getP3()), static_cast<Point2D *>(t.getP1()), &pt);*/
	}
	else if (l3 <= Data::getTolerance())
	{
		return this->straightDistanceEdge(p, 0, 1);
		/*return this->straightDistanceEdge(
			static_cast<Point3D *>(this->getP1()), static_cast<Point3D *>(this->getP2()), p,
			static_cast<Point2D *>(t.getP1()), static_cast<Point2D *>(t.getP2()), &pt);*/
	}

	//std::cout << "ERROR: something is wrong in Real Data::Triangle3D::distance(const Point3D &p) const strikes back" << std::endl;

	return 0.0;
}

Real Data::Triangle3D::distance(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->distance(p);
}

Real Data::Triangle3D::straightDistance(const Point3D &p) const
{
	return this->straightDistance(&p);
}

Real Data::Triangle3D::straightDistance(const Point3D *p) const
{
	return this->height(p);
}

Real Data::Triangle3D::straightDistance(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->straightDistance(p);
}

Real Data::Triangle3D::height(const Point3D &p) const
{
	return this->height(&p);
}

Real Data::Triangle3D::height(const Point3D *p) const
{
	/*Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	return (3.0*p1->volume(p2, p3, p)/this->surface());*/

	Vector3D n = this->normal();
#if USE_EXPERIMENTAL_3
	Vector3D v(this->mid3D(), *p);
#else
	Vector3D v(this->mid(), p);
#endif //#if USE_EXPERIMENTAL_3

	return std::fabs(v.dot(n)/n.norm());
}

Real Data::Triangle3D::height(Real x, Real y, Real z) const
{
	Point3D p(x, y, z);

	return this->height(p);
}

bool Data::Triangle3D::accordingToNormal(const Point3D &p, bool insideTest) const
{
	return this->accordingToNormal(&p, insideTest);
}

bool Data::Triangle3D::accordingToNormal(const Point3D *p, bool insideTest) const
{
	Vector3D n = this->normal();
	Vector3D v(static_cast<Point3D *>(this->getP1()), p);

	//n.normalize();
	v.normalize();

	//since both n and v are normalized, the cosAngle need not to be called, because
	//  n.cosAngle(v) = n.dot(v)/(n.norm()*v.norm())
	//return (insideTest) ? n.cosAngle(v) > -Data::getTolerance() : n.cosAngle(v) > Data::getTolerance();
	return (insideTest) ? n.dot(v) > -Data::getTolerance() : n.dot(v) > Data::getTolerance();
}

Real Data::Triangle3D::solidAngle(const Point3D &p) const
{
	return this->solidAngle(&p);
}

Real Data::Triangle3D::solidAngle(const Point3D *p) const
{
	return std::fabs(this->orientedSolidAngle(p));

	/*//found in http://en.wikipedia.org/wiki/Solid_angle
	// I think the code in Wikipedia is wrong
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	Vector3D a(p, p1);
	Vector3D b(p, p2);
	Vector3D c(p, p3);

	Real den = a.norm()*b.norm()*c.norm() +
				 a.dot(b)*c.norm() +
				 b.dot(c)*a.norm() +
				 c.dot(a)*b.norm();

	if (std::fabs(den) < Data::getTolerance())
	{
		return 0.0;
	}

	Real num = a.dot(c.cross(b));

	Real ang = std::atan2(num, den);

	ang = 2.0*((ang < 0.0) ? ang + M_PI : ang);

	return ang;*/
}

Real Data::Triangle3D::orientedSolidAngle(const Point3D &p, bool insideTest) const
{
	return this->orientedSolidAngle(&p, insideTest);
}

Real Data::Triangle3D::orientedSolidAngle(const Point3D *p, bool insideTest) const
{
	if (!this->out(p))
	{
		return 2.0*M_PI;
	}

	//found in http://en.wikipedia.org/wiki/Solid_angle
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	Vector3D a(p, p1);
	Vector3D b(p, p2);
	Vector3D c(p, p3);

	Real an = a.norm();
	Real bn = b.norm();
	Real cn = c.norm();

	Real den = an*bn*cn + a.dot(b)*cn + b.dot(c)*an + c.dot(a)*bn;

	if (std::fabs(den) < Data::getTolerance())
	{
		return 0.0;
	}

	Real num = a.dot(c.cross(b));

	Real ang = 2.0*std::atan2(num, den);

	if (std::fabs(ang) < Data::getTolerance())
	{
		ang = 0.0;
	}

	//std::cout << "ang = " << ang << " ";

	ang = this->accordingToNormal(p, insideTest) ?
		((ang < 0.0) ?  2.0*M_PI + ang : ang) :
		((ang > 0.0) ? -2.0*M_PI + ang : ang);

	/*if (!this->accordingToNormal(p, insideTest))
	{
		std::cout << "NOT ";
	}

	std::cout << "according to normal, angle = " << ang << std::endl;*/

	return ang;

	/*Real ang = this->solidAngle(p);

	return (this->accordingToNormal(p, insideTest)) ? ang : -ang;*/
}

R3Tuple Data::Triangle3D::barycentricCoordinates(const Point &p) const
{
	return this->barycentricCoordinates(&p);
}

R3Tuple Data::Triangle3D::barycentricCoordinates(const Point *p) const
{
	return this->barycentricCoordinates(static_cast<const Point3D *>(p));
}

R3Tuple Data::Triangle3D::barycentricCoordinates(const Point3D &p) const
{
	return this->barycentricCoordinates(&p);
}

R3Tuple Data::Triangle3D::barycentricCoordinates(const Point3D *p) const
{
	Real s = this->surface();

	if (s < Data::getTolerance())
	{
#if USE_C__11
		return std::make_tuple(0.0, 0.0, 0.0);
#else
		R3Tuple t;
		t.values[0] = t.values[1] = t.values[2] = 0.0;

		return t;
#endif //
	}

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	Real l1 = p->surface(p2, p3)/s;
	Real l2 = p1->surface(p, p3)/s;
	Real l3 = p1->surface(p2, p)/s;

#if USE_C__11
	return std::make_tuple(l1, l2, l3);
#else
	R3Tuple t;
	t.values[0] = l1;
	t.values[1] = l2;
	t.values[2] = l3;

	return t;
#endif //#if USE_C__11
}

Real Data::Triangle3D::surface() const
{
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	return p1->surface(p2, p3);
}

Real Data::Triangle3D::orientedSurface() const
{
	return this->surface();
}

Triangle2D Data::Triangle3D::convert(Real **back, Real **change) const
{
	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	Vector3D i(*p1, *p2);
	Vector3D k = this->normal();

	i.normalize();
	//k.normalize();

	Vector3D j = k.cross(i);

#if USE_EXPERIMENTAL_3
	Vector3D v1(p1);
#else
	Vector v1(p1);
#endif //#if USE_EXPERIMENTAL_3

	Real **coordChange = new Real*[4];

	for (UInt i = 0; i < 4; i++)
	{
		coordChange[i] = new Real[4];
	}

	coordChange[0][0] = i.getX();
	coordChange[0][1] = i.getY();
	coordChange[0][2] = i.getZ();
	coordChange[0][3] = -i.dot(v1);

	coordChange[1][0] = j.getX();
	coordChange[1][1] = j.getY();
	coordChange[1][2] = j.getZ();
	coordChange[1][3] = -j.dot(v1);

	coordChange[2][0] = k.getX();
	coordChange[2][1] = k.getY();
	coordChange[2][2] = k.getZ();
	coordChange[2][3] = -k.dot(v1);

	coordChange[3][0] = 0.0;
	coordChange[3][1] = 0.0;
	coordChange[3][2] = 0.0;
	coordChange[3][3] = 1.0;

	if (back)
	{
		back[0][0] = i.getX();
		back[0][1] = j.getX();
		back[0][2] = k.getX();
		back[0][3] = p1->getX();

		back[1][0] = i.getY();
		back[1][1] = j.getY();
		back[1][2] = k.getY();
		back[1][3] = p1->getY();

		back[2][0] = i.getZ();
		back[2][1] = j.getZ();
		back[2][2] = k.getZ();
		back[2][3] = p1->getZ();

		back[3][0] = 0.0;
		back[3][1] = 0.0;
		back[3][2] = 0.0;
		back[3][3] = 1.0;
	}

	if (change)
	{
		for (UInt i = 0; i < 4; i++)
		{
			for (UInt j = 0; j < 4; j++)
			{
				change[i][j] = coordChange[i][j];
			}
		}
	}

	Point3D newp1(p1), newp2(p2), newp3(p3);

	newp1.transform(coordChange);
	newp2.transform(coordChange);
	newp3.transform(coordChange);

	for (UInt i = 0; i < 4; i++)
	{
		delete [] coordChange[i];
	}

	delete [] coordChange;

	Point2D p2d1(newp1);
	Point2D p2d2(newp2);
	Point2D p2d3(newp3);

	Triangle2D t(p2d1, p2d2, p2d3);

	return t;
}

#if USE_EXPERIMENTAL_3
Point3D Data::Triangle3D::circumcenter3D() const
#else
Point Data::Triangle3D::circumcenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	Real **coordChange = new Real*[4];

	for (UInt i = 0; i < 4; i++)
	{
		coordChange[i] = new Real[4];
	}

#if USE_EXPERIMENTAL_3
    Point2D c2d = this->convert(coordChange).circumcenter2D();
	Point3D center(c2d.getX(), c2d.getY(), 0.0);
#else
	Point3D center(this->convert(coordChange).circumcenter());
#endif //#if USE_EXPERIMENTAL_3

	center.transform(coordChange);

	return center;
}

#if USE_EXPERIMENTAL_3
Point2D Data::Triangle3D::circumcenter2D() const
{
    return Point2D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_EXPERIMENTAL_3
Point3D Data::Triangle3D::incenter3D() const
#else
Point Data::Triangle3D::incenter() const
#endif //#if USE_EXPERIMENTAL_3
{
	Real **coordChange = new Real*[4];

	for (UInt i = 0; i < 4; i++)
	{
		coordChange[i] = new Real[4];
	}

#if USE_EXPERIMENTAL_3
	Point2D c2d = this->convert(coordChange).incenter2D();
	Point3D center(c2d.getX(), c2d.getY(), 0.0);
#else
	Point3D center(this->convert(coordChange).incenter());
#endif //#if USE_EXPERIMENTAL_3

	center.transform(coordChange);

	return center;
}

#if USE_EXPERIMENTAL_3
Point2D Data::Triangle3D::incenter2D() const
{
    return Point2D();
}
#endif //#if USE_EXPERIMENTAL_3

#if USE_GUI
//void Data::Triangle3D::draw(bool fill) const
void Data::Triangle3D::draw() const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return;
	}

	/*if ((!fill) && (!this->isHighlighted()))
	{
		return;
	}*/

	glColor(this->getR(), this->getG(), this->getB());

	Vector3D n = this->normal();
	glNormal(n.getX(), n.getY(), n.getZ());

	Point3D *p1 = static_cast<Point3D *>(this->getP1());
	Point3D *p2 = static_cast<Point3D *>(this->getP2());
	Point3D *p3 = static_cast<Point3D *>(this->getP3());

	bool changed = false;
	int modes[2];

	glGetIntegerv(GL_POLYGON_MODE, modes);

	bool wireframe = ((modes[0] != GL_FILL) || (modes[1] != GL_FILL));

	if ((this->isHighlighted()) && (wireframe))
	{
		changed = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glPushMatrix();
#if USE_EXPERIMENTAL_3
		this->explodeAndShrink(this->mid3D());
#else
		this->explodeAndShrink(this->mid());
#endif //#if USE_EXPERIMENTAL_3

		glBegin(GL_TRIANGLES);
			glVertex(p1->getX(), p1->getY(), p1->getZ());
			glVertex(p2->getX(), p2->getY(), p2->getZ());
			glVertex(p3->getX(), p3->getY(), p3->getZ());
		glEnd();

		if (!wireframe)
		{
			GLboolean lighting = glIsEnabled(GL_LIGHTING);

			if (lighting)
			{
				glDisable(GL_LIGHTING);
			}

			glLineWidth(static_cast<GLfloat>(Data::getState().getLineWidth()));

			glBegin(GL_LINE_LOOP);
				glVertex(p1->getX(), p1->getY(), p1->getZ());
				glVertex(p2->getX(), p2->getY(), p2->getZ());
				glVertex(p3->getX(), p3->getY(), p3->getZ());
			glEnd();

			if (lighting)
			{
				glEnable(GL_LIGHTING);
			}
		}
	glPopMatrix();

	if (changed)
	{
		glPolygonMode(GL_FRONT, modes[0]);
		glPolygonMode(GL_BACK, modes[1]);
	}
}

UInt Data::Triangle3D::fill(GLfloat *coord, GLfloat *color, GLfloat *normal) const
{
    return this->fill(coord, color, normal, NULL);
}

UInt Data::Triangle3D::fill(GLfloat *coord, GLfloat *color, GLfloat *normal, Data::PolygonList *adjacency) const
{
	if (!Data::getState().isInVisibleSpace(this))
	{
		return 0;
	}

	if (color)
	{
		color[0] = color[3] = color[6] = static_cast<GLfloat>(this->getR());
		color[1] = color[4] = color[7] = static_cast<GLfloat>(this->getG());
		color[2] = color[5] = color[8] = static_cast<GLfloat>(this->getB());
	}

	if (normal)
	{
		if (adjacency)
        {
            Vector3D n[3];

            for (UInt i = 0; i < 3; i++)
            {
                if (adjacency[i].empty())
                {
                    n[i] = this->normal();

                    continue;
                }

                for (Data::PolygonList::iterator iter = adjacency[i].begin();
                     iter != adjacency[i].end(); iter++)
                {
                    Data::Vector3D normal = (*iter)->normal();

                    normal.normalize();

                    UInt j = 0;

                    for (j = 0; j < (*iter)->numPoints(); j++)
                    {
                        if ((*iter)->getPoint(j) == this->getPoint(i))
                        {
                            break;
                        }
                    }

                    if (j == (*iter)->numPoints())
                    {
                        n[i].sum(normal);

                        continue;
                    }

                    UInt next = (j + 1)%(*iter)->numPoints();
                    UInt prev = (j + (*iter)->numPoints() - 1)%(*iter)->numPoints();

                    Data::Vector3D v1(this->getPoint(i), (*iter)->getPoint(next));
                    Data::Vector3D v2(this->getPoint(i), (*iter)->getPoint(prev));

                    Real ang = v1.angle(v2);

                    normal.multiply(ang);

                    n[i].sum(normal);
                }
            }

            for (UInt i = 0; i < 3; i++)
            {
                n[i].normalize();

                normal[3*i + 0] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[i].getX()) : static_cast<GLfloat>(n[i].getX());
                normal[3*i + 1] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[i].getY()) : static_cast<GLfloat>(n[i].getY());
                normal[3*i + 2] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n[i].getZ()) : static_cast<GLfloat>(n[i].getZ());
            }
        }
        else
        {
            Vector3D n = this->normal();

            normal[0] = normal[3] = normal[6] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getX()) : static_cast<GLfloat>(n.getX());
            normal[1] = normal[4] = normal[7] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getY()) : static_cast<GLfloat>(n.getY());
            normal[2] = normal[5] = normal[8] = (Data::getState().isNormalInverted()) ? -static_cast<GLfloat>(n.getZ()) : static_cast<GLfloat>(n.getZ());
        }
	}

	coord[0] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getX());
	coord[1] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getY());
	coord[2] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP1())->getZ());

	coord[3] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getX());
	coord[4] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getY());
	coord[5] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP2())->getZ());

	coord[6] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getX());
	coord[7] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getY());
	coord[8] = static_cast<GLfloat>(static_cast<Point3D *>(this->getP3())->getZ());

#if USE_EXPERIMENTAL_3
	Point3D mid = this->mid3D();
#else
	Point mid = this->mid();
#endif //#if USE_EXPERIMENTAL_3

	this->explodeAndShrink(coord, mid);
	this->explodeAndShrink(coord + 3, mid);
	this->explodeAndShrink(coord + 6, mid);

	return 3;
}
#endif //#if USE_GUI
