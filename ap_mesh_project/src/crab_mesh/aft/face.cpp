#include "../../../include/crab_mesh/aft/face.h"

#include <memory>

Face::Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
           std::shared_ptr<Vertex> v3, long int id)
    : Shape(id) {
  mid = nullptr;

  setVertices(v1, v2, v3);

  h = 0.0;
}

Face::Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
           std::shared_ptr<Vertex> v3, std::shared_ptr<Vertex> mid, long int id)
    : Shape(id) {
  this->mid = nullptr;

  setVertices(v1, v2, v3);

  this->mid = mid;

  h = 0.0;
}

std::shared_ptr<Vertex> Face::makeMid() {
  std::shared_ptr<Vertex> m = std::make_shared<Vertex>();

  if (v[0] && v[1] && v[2]) {
    m->setPosition((v[0]->getX() + v[1]->getX() + v[2]->getX()) / 3.0,
                   (v[0]->getY() + v[1]->getY() + v[2]->getY()) / 3.0);

    // cout << "triangulo " << id << ", meio = ( " << m->getX() << ", " <<
    // m->getY() << " )" << endl;
  }

  return m;
}

void Face::setV1(std::shared_ptr<Vertex> v) { this->v[0] = v; }

void Face::setV2(std::shared_ptr<Vertex> v) { this->v[1] = v; }

void Face::setV3(std::shared_ptr<Vertex> v) { this->v[2] = v; }

void Face::setVertices(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
                       std::shared_ptr<Vertex> v3) {
  setV1(v1);
  setV2(v2);
  setV3(v3);

  if (mid) {
    mid = nullptr;
  }

  mid = makeMid();
}

std::shared_ptr<Vertex> Face::getV1() { return v[0]; }

std::shared_ptr<Vertex> Face::getV2() { return v[1]; }

std::shared_ptr<Vertex> Face::getV3() { return v[2]; }

std::shared_ptr<Vertex> Face::getMid() { return mid; }

std::shared_ptr<Vertex> Face::circumcenter() {
  /*static*/ double x1, y1, x2, y2, x3, y3, x, y;
  /*static*/ double x12PlusY12, x22PlusY22, x32PlusY32;
  /*static*/ double detNum1, detNum2, detDen, den;

  x1 = v[0]->getX();
  y1 = v[0]->getY();
  x2 = v[1]->getX();
  y2 = v[1]->getY();
  x3 = v[2]->getX();
  y3 = v[2]->getY();

  x12PlusY12 = x1 * x1 + y1 * y1;
  x22PlusY22 = x2 * x2 + y2 * y2;
  x32PlusY32 = x3 * x3 + y3 * y3;

  detNum1 = x12PlusY12 * y2 + x22PlusY22 * y3 + x32PlusY32 * y1 -
            x32PlusY32 * y2 - x22PlusY22 * y1 - x12PlusY12 * y3;
  detNum2 = x12PlusY12 * x3 + x22PlusY22 * x1 + x32PlusY32 * x2 -
            x32PlusY32 * x1 - x22PlusY22 * x3 - x12PlusY12 * x2;
  detDen = x1 * y2 + x2 * y3 + x3 * y1 - x3 * y2 - x1 * y3 - x2 * y1;
  den = 2.0 * detDen;

  x = detNum1 / den;
  y = detNum2 / den;

  std::shared_ptr<Vertex> center = std::make_shared<Vertex>(x, y);

  return center;
}

std::shared_ptr<Vertex> Face::incenter() {
  /*static*/ double a, b, c, sum, x, y;

  a = v[0]->distance(v[1]);
  b = v[1]->distance(v[2]);
  c = v[2]->distance(v[0]);

  sum = a + b + c;

  x = (a * v[2]->getX() + b * v[0]->getX() + c * v[1]->getX()) / sum;
  y = (a * v[2]->getY() + b * v[0]->getY() + c * v[1]->getY()) / sum;

  std::shared_ptr<Vertex> center = std::make_shared<Vertex>(x, y);

  return center;
}

double Face::surface() { return v[0]->surface(v[1], v[2]); }

double Face::orientedSurface() { return v[0]->orientedSurface(v[1], v[2]); }

bool Face::in(double x, double y) {
  /*static*/ Vertex v;

  v.setPosition(x, y);

  return in(std::make_shared<Vertex>(v));
}

bool Face::in(std::shared_ptr<Vertex> v) {
  /*static*/ double l1, l2, l3;

  l1 = l2 = l3 = 0.0;

  barycentricCoordinates(v, l1, l2, l3);

  // return ((l1 >= 0.0) && (l2 >= 0.0) && (l3 >= 0.0));
  return ((l1 > 0.0) && (l2 > 0.0) && (l3 > 0.0));
}

bool Face::on(std::shared_ptr<Vertex> v) { return (!in(v) && !out(v)); }

bool Face::out(double x, double y) {
  /*static*/ Vertex v;

  v.setPosition(x, y);

  return out(std::make_shared<Vertex>(v));
}

bool Face::out(std::shared_ptr<Vertex> v) {
  /*static*/ double l1, l2, l3;

  l1 = l2 = l3 = 0.0;

  // #pragma omp critical
  //       cout<<v->getText()<<omp_get_thread_num()<<endl;

  barycentricCoordinates(v, l1, l2, l3);

  // return ((l1 >= 0.0) && (l2 >= 0.0) && (l3 >= 0.0));
  return ((l1 < -TOLERANCE_AFT) || (l2 < -TOLERANCE_AFT) ||
          (l3 < -TOLERANCE_AFT));
}

void Face::barycentricCoordinates(std::shared_ptr<Vertex> v, double &l1,
                                  double &l2, double &l3) {
  /*static*/ double s;
  // #pragma omp critical
  //     {
  //         cout<<this->v[0]->getText()<<" thread:
  //         "<<omp_get_thread_num()<<endl; cout<<this->v[1]->getText()<<"
  //         thread: "<<omp_get_thread_num()<<endl;
  //         cout<<this->v[2]->getText()<<" thread:
  //         "<<omp_get_thread_num()<<endl;

  //    }
  s = this->v[0]->orientedSurface(this->v[1], this->v[2]);

  l1 = v->orientedSurface(this->v[1], this->v[2]) / s;
  l2 = this->v[0]->orientedSurface(v, this->v[2]) / s;
  l3 = this->v[0]->orientedSurface(this->v[1], v) / s;
}

double Face::gama() {
  /*static*/ double d1, d2, d3;

  d1 = v[0]->distance(v[1]);
  d2 = v[1]->distance(v[2]);
  d3 = v[2]->distance(v[0]);

  return (pow(sqrt((d1 * d1 + d2 * d2 + d3 * d3) / 3.0), 2.0) / surface());
}

double Face::quality() {
  /*static*/ double rInsc, rCirc, d;
  /*static*/ Edge e;

  rInsc = DBL_MAX;
  rCirc = DBL_MIN;

  std::shared_ptr<Vertex> insc = incenter();
  std::shared_ptr<Vertex> circ = circumcenter();

  e.setVertices(v[0], v[1]);

  d = e.straightDistance(insc);

  rInsc = (d < rInsc) ? d : rInsc;

  e.setVertices(v[1], v[2]);

  d = e.straightDistance(insc);

  rInsc = (d < rInsc) ? d : rInsc;

  e.setVertices(v[2], v[0]);

  d = e.straightDistance(insc);

  rInsc = (d < rInsc) ? d : rInsc;

  d = circ->distance(v[0]);

  rCirc = (d > rCirc) ? d : rCirc;

  d = circ->distance(v[1]);

  rCirc = (d > rCirc) ? d : rCirc;

  d = circ->distance(v[2]);

  rCirc = (d > rCirc) ? d : rCirc;

  e.setVertices(nullptr, nullptr);

  insc = nullptr;
  circ = nullptr;

  return 2.0 * rInsc / rCirc;
}

bool Face::isBad() {
  static const double gamaEquil = 4.0 / sqrt(3.0);

  // gama/gama* = gama()/gama(de um triangulo equilatero de lado 1)
  return (gama() / gamaEquil > 1.5);
}

bool Face::hasEdge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  return (((this->v[0] == v1) && (this->v[1] == v2)) ||
          ((this->v[0] == v2) && (this->v[1] == v1)) ||
          ((this->v[1] == v1) && (this->v[2] == v2)) ||
          ((this->v[1] == v2) && (this->v[2] == v1)) ||
          ((this->v[2] == v1) && (this->v[0] == v2)) ||
          ((this->v[2] == v2) && (this->v[0] == v1)));
}

bool Face::hasEdge(std::shared_ptr<Edge> e) {
  return hasEdge(e->getV1(), e->getV2());
}

bool Face::isAdjacent(std::shared_ptr<Face> f) {
  if ((f == nullptr) || (f == this->shared_from_this())) {
    return false;
  }

  return ((hasEdge(f->v[0], f->v[1])) || (hasEdge(f->v[1], f->v[2])) ||
          (hasEdge(f->v[2], f->v[0])));
}

bool Face::hits(std::shared_ptr<Face> f) {
  /*static*/ bool intercept = false;

  if ((in(f->v[0])) || (in(f->v[1])) || (in(f->v[2])) || (f->in(v[0])) ||
      (f->in(v[1])) || (f->in(v[2]))) {
    intercept = true;
  }

  return intercept;
}

bool Face::hits(std::shared_ptr<Edge> e) {
  return ((e->intercept(v[0], v[1])) || (e->intercept(v[1], v[2])) ||
          (e->intercept(v[2], v[0])));
}

string Face::getText() {
  ostringstream stream;

  stream << id << " " << v[0]->getId() << " " << v[1]->getId() << " "
         << v[2]->getId();

  string s = stream.str();

  return s;
}
