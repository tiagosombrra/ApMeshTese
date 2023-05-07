#include "../../../include/crab_mesh/aft/edge.h"

extern double kToleranceAft;

Edge::Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, long int id)
    : Shape(id),
      mid(nullptr),
      vector(nullptr),
      len(-1.0),
      xmin(DBL_MIN),
      xmax(DBL_MAX),
      ymin(DBL_MIN),
      ymax(DBL_MAX) {
  setVertices(v1, v2);
  setCell(nullptr);
  setFree(true);
  setInBoundary(false);
  setCurva(nullptr);
}

Edge::~Edge() {}

std::shared_ptr<Vertex> Edge::makeMid() {
  auto v = std::make_shared<Vertex>();

  if (this->v[0] && this->v[1]) {
    v->setPosition((this->v[0]->getX() + this->v[1]->getX()) / 2.0,
                   (this->v[0]->getY() + this->v[1]->getY()) / 2.0);
  }

  return v;
}

std::shared_ptr<Vertex> Edge::makeVector() {
  auto v = std::make_shared<Vertex>();
  if (this->v[0] && this->v[1]) {
    v->setPosition(this->v[1]->getX() - this->v[0]->getX(),
                   this->v[1]->getY() - this->v[0]->getY());
  }

  return v;
}

void Edge::setV1(std::shared_ptr<Vertex> v) { this->v[0] = v; }

void Edge::setV2(std::shared_ptr<Vertex> v) { this->v[1] = v; }

void Edge::setVertices(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  setV1(v1);
  setV2(v2);

  if (v1 && v2) {
    if (v1->getX() > v2->getX()) {
      xmin = v2->getX();
      xmax = v1->getX();
    } else {
      xmin = v1->getX();
      xmax = v2->getX();
    }

    if (v1->getY() > v2->getY()) {
      ymin = v2->getY();
      ymax = v1->getY();
    } else {
      ymin = v1->getY();
      ymax = v2->getY();
    }
  }

  if (mid) {
    mid = nullptr;
  }

  if (vector) {
    vector = nullptr;
  }

  mid = makeMid();
  vector = makeVector();
}

std::shared_ptr<Vertex> Edge::getV1() { return v[0]; }

std::shared_ptr<Vertex> Edge::getV2() { return v[1]; }

void Edge::setInBoundary(bool inBoundary) { this->inBoundary = inBoundary; }

bool Edge::isInBoundary() { return inBoundary; }

void Edge::setFree(bool free) { this->free = free; }

bool Edge::isFree() { return free; }

void Edge::setCell(QuadtreeCell* cell) { this->cell = cell; }

QuadtreeCell* Edge::getCell() { return cell; }

std::shared_ptr<Vertex> Edge::getMid() { return mid; }

double Edge::length() { return vector->norm(); }

bool Edge::intercept(std::shared_ptr<Edge> e) {
  return intercept(e->v[0], e->v[1]);
}

bool Edge::intercept(std::shared_ptr<Vertex> va, std::shared_ptr<Vertex> vb) {
  double exmin, exmax, eymin, eymax;

  if (va->getX() >= vb->getX()) {
    exmax = va->getX();
    exmin = vb->getX();
  } else {
    exmax = vb->getX();
    exmin = va->getX();
  }

  if ((xmin > exmax) || (xmax < exmin)) {
    return false;
  }

  if (va->getY() >= vb->getY()) {
    eymax = va->getY();
    eymin = vb->getY();
  } else {
    eymax = vb->getY();
    eymin = va->getY();
  }

  if ((ymin > eymax) || (ymax < eymin)) {
    return false;
  }

  // teste de intersecao
  Vertex ab, ac, ad, cd, ca, cb;
  double tivx, tivy, tfvx, tfvy, eivx, eivy, efvx, efvy;

  tivx = v[0]->getX();
  tivy = v[0]->getY();
  tfvx = v[1]->getX();
  tfvy = v[1]->getY();

  eivx = va->getX();
  eivy = va->getY();
  efvx = vb->getX();
  efvy = vb->getY();

  ab.setPosition(tfvx - tivx, tfvy - tivy);
  ac.setPosition(eivx - tivx, eivy - tivy);
  ad.setPosition(efvx - tivx, efvy - tivy);

  cd.setPosition(efvx - eivx, efvy - eivy);
  ca.setPosition(tivx - eivx, tivy - eivy);
  cb.setPosition(tfvx - eivx, tfvy - eivy);

  return ((ab.cross(ac) * ab.cross(ad) < 0) &&
          (cd.cross(ca) * cd.cross(cb) < 0));
}

bool Edge::intercept(Vertex v) { return (distance(v) < kToleranceAft); }

bool Edge::intercept(double x, double y) {
  Vertex v;

  v.setPosition(x, y);

  return intercept(v);
}

double Edge::straightDistance(Vertex v) {
  return 2.0 * v.surface(*this->v[0].get(), *this->v[1].get()) / length();
}

double Edge::distance(Vertex v) {
  Vertex ac, vec;

  ac.setPosition(v.getX() - this->v[0]->getX(), v.getY() - this->v[0]->getY());
  vec.setPosition(this->v[1]->getX() - this->v[0]->getX(),
                  this->v[1]->getY() - this->v[0]->getY());

  ac.scalarMultiplication(1.0 / ac.norm());
  vec.scalarMultiplication(1.0 / vec.norm());

  if (ac.dot(vec) < kToleranceAft) {
    return this->v[0]->distance(v);
  }

  ac.setPosition(v.getX() - this->v[1]->getX(), v.getY() - this->v[1]->getY());

  ac.scalarMultiplication(1.0 / ac.norm());
  vec.scalarMultiplication(-1.0);

  if (ac.dot(vec) < kToleranceAft) {
    return this->v[1]->distance(v);
  }

  return straightDistance(v);
}

double Edge::distance(double x, double y) {
  Vertex v;

  v.setPosition(x, y);

  return distance(v);
}

double Edge::dot(std::shared_ptr<Vertex> v) {
  Vertex vAux;

  vAux.setPosition(v->getX() - this->v[0]->getX(),
                   v->getY() - this->v[0]->getY());

  return vector->dot(vAux);
}

bool Edge::left(std::shared_ptr<Vertex> v) {
  return left(v->getX(), v->getY());
}

bool Edge::left(double x, double y) {
  Vertex vAux;

  vAux.setPosition(x - this->v[0]->getX(), y - this->v[0]->getY());

  return vAux.right(*vector.get());
}

bool Edge::right(std::shared_ptr<Vertex> v) {
  return right(v->getX(), v->getY());
}

bool Edge::right(double x, double y) {
  Vertex vAux;

  vAux.setPosition(x - this->v[0]->getX(), y - this->v[0]->getY());

  return vAux.left(*vector.get());
}

bool Edge::accordingToNormal(std::shared_ptr<Vertex> v, bool inEdgeTest) {
  if (inEdgeTest) {
    return !right(v);
  }

  return left(v);
}

double Edge::angle(Edge e) {
  Vertex va, vb, vc;

  if (v[0]->matches(e.v[0])) {
    va = *v[0];
    vb = *v[1];
    vc = *(e.v[1]);
  } else if (v[0]->matches(e.v[1])) {
    va = *v[0];
    vb = *v[1];
    vc = *(e.v[0]);
  } else if (v[1]->matches(e.v[0])) {
    va = *v[1];
    vb = *v[0];
    vc = *(e.v[1]);
  } else if (v[1]->matches(e.v[1])) {
    va = *v[1];
    vb = *v[0];
    vc = *(e.v[0]);
  } else {
    return -1.0;
  }

  vb.setPosition(vb.getX() - va.getX(), vb.getY() - va.getY());
  vc.setPosition(vc.getX() - va.getX(), vc.getY() - va.getY());

  return vb.angle(vc);
}

double Edge::angle(std::shared_ptr<Vertex> va, std::shared_ptr<Vertex> vb) {
  Edge e;

  e.setVertices(va, vb);

  return angle(e);
}

std::shared_ptr<Vertex> Edge::normal() {
  auto normal = std::make_shared<Vertex>();

  if (v[0]->matches(v[1])) {
    normal->setPosition(0.0, 0.0);
  } else if (v[0]->getX() == v[1]->getX()) {
    if (v[0]->getY() > v[1]->getY()) {
      normal->setPosition(1.0, 0.0);
    } else {
      normal->setPosition(-1.0, 0.0);
    }
  } else if (v[0]->getY() == v[1]->getY()) {
    if (v[0]->getX() > v[1]->getX()) {
      normal->setPosition(0.0, -1.0);
    } else {
      normal->setPosition(0.0, 1.0);
    }
  } else {
    normal->setPosition(v[0]->getY() - v[1]->getY(),
                        v[1]->getX() - v[0]->getX());

    double n = normal->norm();

    normal->setPosition(normal->getX() / n, normal->getY() / n);
  }

  return normal;
}

bool Edge::equals(std::shared_ptr<Edge> e) {
  return (((v[0] == e->v[0]) && (v[1] == e->v[1])) ||
          ((v[0] == e->v[1]) && (v[1] == e->v[0])));
}

bool Edge::equals(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  return (((this->v[0] == v1) && (this->v[1] == v2)) ||
          ((this->v[0] == v2) && (this->v[1] == v1)));
}

bool Edge::matches(std::shared_ptr<Edge> e) {
  return ((v[0] == e->v[0]) && (v[1] == e->v[1]));
}

bool Edge::matches(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  return ((this->v[0] == v1) && (this->v[1] == v2));
}

void Edge::setCurva(std::shared_ptr<CurveAdaptiveParametric> c) { this->c = c; }

void Edge::setLen(double len) { this->len = len; }

double Edge::getLen() { return len; }

void Edge::makeParamMid() {
  if (!c) {
    return;
  }

  if (mid) {
    mid = nullptr;
  }

  double t = -1.0;

  if ((v[0]->getY() == 0.0) && (v[0]->getX() < 1.0)) {
    t = c->CalculateMidparameterByParamters(v[0]->getX(), v[1]->getX());
    mid = std::make_shared<Vertex>(t, 0.0);
    len = c->CalculateParametricLength(v[0]->getX(), v[1]->getX()) /
          c->GetLength();
  } else if ((v[0]->getX() == 1.0) && (v[0]->getY() < 1.0)) {
    t = c->CalculateMidparameterByParamters(v[0]->getY(), v[1]->getY());
    mid = std::make_shared<Vertex>(1.0, t);
    len = c->CalculateParametricLength(v[0]->getY(), v[1]->getY()) /
          c->GetLength();
  } else if ((v[0]->getY() == 1.0) && (v[0]->getX() > 0.0)) {
    t = c->CalculateMidparameterByParamters(v[1]->getX(), v[0]->getX());
    mid = std::make_shared<Vertex>(t, 1.0);
    len = c->CalculateParametricLength(v[1]->getX(), v[0]->getX()) /
          c->GetLength();
  } else if ((v[0]->getX() == 0.0) && (v[0]->getY() > 0.0)) {
    t = c->CalculateMidparameterByParamters(v[1]->getY(), v[0]->getY());
    mid = std::make_shared<Vertex>(0.0, t);
    len = c->CalculateParametricLength(v[1]->getY(), v[0]->getY()) /
          c->GetLength();
  } else {
    mid = makeMid();
    len = -1.0;
  }
}

string Edge::getText() {
  ostringstream stream;

  stream << id << " " << v[0]->getId() << " " << v[1]->getId();

  string s = stream.str();

  return s;
}
