#include "../../../include/crab_mesh/aft/face.h"

extern double kToleranceAft;

Face::Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
           std::shared_ptr<Vertex> v3, long int id)
    : Shape(id), mid(nullptr), h(0.0) {
  setVertices(v1, v2, v3);
}

Face::Face(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
           std::shared_ptr<Vertex> v3, std::shared_ptr<Vertex> mid, long int id)
    : Shape(id), mid(mid), h(0.0) {
  setVertices(v1, v2, v3);
}

Face::~Face() {}

std::shared_ptr<Vertex> Face::makeMid() {
  auto m = std::make_shared<Vertex>();

  if (v[0] && v[1] && v[2]) {
    m->setPosition((v[0]->getX() + v[1]->getX() + v[2]->getX()) / 3.0,
                   (v[0]->getY() + v[1]->getY() + v[2]->getY()) / 3.0);
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
  const double x1 = v[0]->getX(), y1 = v[0]->getY();
  const double x2 = v[1]->getX(), y2 = v[1]->getY();
  const double x3 = v[2]->getX(), y3 = v[2]->getY();

  const double A = x1 - x3, B = y1 - y3;
  const double C = x2 - x3, D = y2 - y3;
  const double E = A * (x1 + x3) + B * (y1 + y3);
  const double F = C * (x2 + x3) + D * (y2 + y3);
  const double G = 2.0 * (A * (y2 - y3) - B * (x2 - x3));

  if (G == 0.0) {
    // Triângulo degenerado (colinear)
    return nullptr;
  }

  const double cx = (D * E - B * F) / G;
  const double cy = (A * F - C * E) / G;

  return std::make_shared<Vertex>(cx, cy);
}

std::shared_ptr<Vertex> Face::incenter() {
  const auto sum =
      v[0]->distance(v[1]) + v[1]->distance(v[2]) + v[2]->distance(v[0]);
  const auto x = (v[0]->distance(v[1]) * v[2]->getX() +
                  v[1]->distance(v[2]) * v[0]->getX() +
                  v[2]->distance(v[0]) * v[1]->getX()) /
                 sum;
  const auto y = (v[0]->distance(v[1]) * v[2]->getY() +
                  v[1]->distance(v[2]) * v[0]->getY() +
                  v[2]->distance(v[0]) * v[1]->getY()) /
                 sum;

  return std::make_shared<Vertex>(x, y);
}

double Face::surface() { return v[0]->surface(*v[1].get(), *v[2].get()); }

double Face::orientedSurface() {
  return v[0]->orientedSurface(*v[1].get(), *v[2].get());
}

bool Face::in(double x, double y) {
  Vertex v;
  v.setPosition(x, y);

  return in(v);
}

bool Face::in(Vertex v) {
  double l1, l2, l3;
  l1 = l2 = l3 = 0.0;

  barycentricCoordinates(v, l1, l2, l3);

  return ((l1 > 0.0) && (l2 > 0.0) && (l3 > 0.0));
}

bool Face::on(Vertex v) { return (!in(v) && !out(v)); }

bool Face::out(double x, double y) {
  Vertex v;

  v.setPosition(x, y);

  return out(v);
}

bool Face::out(Vertex v) {
  double l1, l2, l3;

  l1 = l2 = l3 = 0.0;

  barycentricCoordinates(v, l1, l2, l3);

  return ((l1 < -kToleranceAft) || (l2 < -kToleranceAft) ||
          (l3 < -kToleranceAft));
}

void Face::barycentricCoordinates(Vertex v, double &l1, double &l2,
                                  double &l3) {
  double s;
  s = this->v[0]->orientedSurface(*this->v[1].get(), *this->v[2].get());
  l1 = v.orientedSurface(*this->v[1].get(), *this->v[2].get()) / s;
  l2 = this->v[0]->orientedSurface(v, *this->v[2].get()) / s;
  l3 = this->v[0]->orientedSurface(*this->v[1].get(), v) / s;
}

double Face::gama() {
  double d1, d2, d3;
  double sum = 0.0;

  d1 = v[0]->distance(v[1]);
  d2 = v[1]->distance(v[2]);
  d3 = v[2]->distance(v[0]);

  sum = (d1 * d1 + d2 * d2 + d3 * d3) / 3.0;

  return (sqrt(sum) / surface()) * (sqrt(sum) / surface());
}

double Face::quality() {
  double rInsc, rCirc, d;
  Edge e;

  rInsc = DBL_MAX;
  rCirc = DBL_MIN;

  auto insc = incenter();
  auto circ = circumcenter();

  e.setVertices(v[0], v[1]);

  d = e.straightDistance(*insc.get());

  rInsc = (d < rInsc) ? d : rInsc;

  e.setVertices(v[1], v[2]);

  d = e.straightDistance(*insc.get());

  rInsc = (d < rInsc) ? d : rInsc;

  e.setVertices(v[2], v[0]);

  d = e.straightDistance(*insc.get());

  rInsc = (d < rInsc) ? d : rInsc;

  d = circ->distance(v[0]);

  rCirc = (d > rCirc) ? d : rCirc;

  d = circ->distance(v[1]);

  rCirc = (d > rCirc) ? d : rCirc;

  d = circ->distance(v[2]);

  rCirc = (d > rCirc) ? d : rCirc;

  e.setVertices(nullptr, nullptr);

  return 2.0 * rInsc / rCirc;
}

bool Face::isBad() {
  static const double gamaEquil = 4.0 / sqrt(3.0);

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
  if ((f == nullptr) || (f.get() == this)) {
    return false;
  }

  return ((hasEdge(f->v[0], f->v[1])) || (hasEdge(f->v[1], f->v[2])) ||
          (hasEdge(f->v[2], f->v[0])));
}

bool Face::hits(std::shared_ptr<Face> f) {
  bool intercept = false;

  if ((in(*f->v[0].get())) || (in(*f->v[1].get())) || (in(*f->v[2].get())) ||
      (f->in(*v[0].get())) || (f->in(*v[1].get())) || (f->in(*v[2].get()))) {
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
