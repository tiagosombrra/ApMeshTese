#include "../../../include/crab_mesh/aft/edge.h"

#include <cstdint>
#include <memory>

Edge::Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
           std::uint64_t id)
    : Shape(id) {
  mid = vector = nullptr;

  xmax = ymax = DBL_MAX;
  xmin = ymin = DBL_MIN;

  len = -1.0;

  setVertices(v1, v2);
  setCell(nullptr);
  setFree(true);
  setInBoundary(false);
  setCurva(nullptr);
}

std::shared_ptr<Vertex> Edge::makeMid() {
  std::shared_ptr<Vertex> v = std::make_shared<Vertex>();

  if (this->v[0] && this->v[1]) {
    v->setPosition((this->v[0]->getX() + this->v[1]->getX()) / 2.0,
                   (this->v[0]->getY() + this->v[1]->getY()) / 2.0);
  }

  return v;
}

std::shared_ptr<Vertex> Edge::makeVector() {
  std::shared_ptr<Vertex> v = std::make_shared<Vertex>();

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

// #if USE_OPENGL
//  void Edge::setWidth(double width)
//{
//     this->width = width;
// }
// #endif //#if USE_OPENGL

void Edge::setFree(bool free) { this->free = free; }

bool Edge::isFree() { return free; }

void Edge::setCell(std::shared_ptr<QuadtreeCell> cell) { this->cell = cell; }

std::shared_ptr<QuadtreeCell> Edge::getCell() { return cell; }

std::shared_ptr<Vertex> Edge::getMid() { return mid; }

double Edge::length() { return vector->norm(); }

bool Edge::intercept(std::shared_ptr<Edge> e) {
  return intercept(e->v[0], e->v[1]);
}

bool Edge::intercept(std::shared_ptr<Vertex> va, std::shared_ptr<Vertex> vb) {
  /*if ((va == v[0]) || (va == v[1]) || (vb == v[0]) || (vb == v[1]))
  {
      return false;
  }*/

  // teste de bounding boxes
  /*static*/ double exmin, exmax, eymin, eymax;

  if (va->getX() >= vb->getX()) {
    exmax = va->getX();
    exmin = vb->getX();
  } else {
    exmax = vb->getX();
    exmin = va->getX();
  }

  /*//testa se os intervalos das bounding boxes se interceptam no eixo x
  if (((txmin < exmin) || (txmin > exmax)) &&
      ((txmax < exmin) || (txmax > exmax)))
  {
      return false;
  }*/

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
  /*static*/ Vertex ab, ac, ad, cd, ca, cb;
  /*static*/ double tivx, tivy, tfvx, tfvy, eivx, eivy, efvx, efvy;

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

  return ((ab.cross(std::make_shared<Vertex>(ac)) *
               ab.cross(std::make_shared<Vertex>(ad)) <
           0) &&
          (cd.cross(std::make_shared<Vertex>(ca)) *
               cd.cross(std::make_shared<Vertex>(cb)) <
           0));
}

bool Edge::intercept(std::shared_ptr<Vertex> v) {
  return (distance(v) < TOLERANCE_AFT);

  /*static Vertex ac, bc;

  ac.setPosition(v->getX() - this->v[0]->getX(), v->getY() -
  this->v[0]->getY()); bc.setPosition(v->getX() - this->v[1]->getX(), v->getY()
  - this->v[1]->getY());

  return ((ac.dot(vector) > 0.0) &&
          (ac.dot(vector) > 0.0) &&
          (straightDistance(v) > tolerance));*/

  /*static double distAB;

  distAB = length() + tolerance;

  return ((fabs(vector->cross(&ac)) < tolerance) &&
          (this->v[0]->distance(v) <= distAB) &&
          (this->v[1]->distance(v) <= distAB));*/
}

bool Edge::intercept(double x, double y) {
  /*static*/ Vertex v;

  v.setPosition(x, y);

  return intercept(std::make_shared<Vertex>(v));
}

double Edge::straightDistance(std::shared_ptr<Vertex> v) {
  return 2.0 * v->surface(this->v[0], this->v[1]) / length();

  /*static Vertex w, p;
  static double c1, c2, b;

  w.setPosition(v->getX() - this->v[0]->getX(), v->getY() - this->v[0]->getY());

  c1 = w.dot(vector);
  c2 = vector->dot(vector);

  if (c2 <= tolerance)
  {
      return -1.0;
  }

  b = c1/c2;

  p.setPosition(this->v[0]->getX() + b*vector->getX(), this->v[0]->getY() +
  b*vector->getY());

  return p.distance(v);*/
}

double Edge::distance(std::shared_ptr<Vertex> v) {
  /*static*/ Vertex ac, vec;

  ac.setPosition(v->getX() - this->v[0]->getX(),
                 v->getY() - this->v[0]->getY());
  vec.setPosition(this->v[1]->getX() - this->v[0]->getX(),
                  this->v[1]->getY() - this->v[0]->getY());

  ac.scalarMultiplication(1.0 / ac.norm());
  vec.scalarMultiplication(1.0 / vec.norm());

  if (ac.dot(std::make_shared<Vertex>(vec)) < TOLERANCE_AFT) {
    return this->v[0]->distance(v);
  }

  ac.setPosition(v->getX() - this->v[1]->getX(),
                 v->getY() - this->v[1]->getY());

  ac.scalarMultiplication(1.0 / ac.norm());
  vec.scalarMultiplication(-1.0);

  if (ac.dot(std::make_shared<Vertex>(vec)) < TOLERANCE_AFT) {
    return this->v[1]->distance(v);
  }

  return straightDistance(v);

  /*static Vertex w, p;
  static double c1, c2, b;

  w.setPosition(v->getX() - this->v[0]->getX(), v->getY() - this->v[0]->getY());

  c1 = w.dot(vector);
  c2 = vector->dot(vector);

  if (c1 < 0.0)
  {
      return this->v[0]->distance(v);
  }

  if (c1 > c2)
  {
      return this->v[1]->distance(v);
  }

  b = c1/c2;

  p.setPosition(this->v[0]->getX() + b*vector->getX(), this->v[0]->getY() +
  b*vector->getY());

  return p.distance(v);*/

  /*if (c1 <= tolerance)
  {
      return this->v[0]->distance(v);
  }
  else if (c2 <= c1)
  {
      return this->v[1]->distance(v);
  }
  else
  {
      b = c1/c2;

      p.setPosition(this->v[0]->getX() + b*vector->getX(), this->v[0]->getY() +
  b*vector->getY());

      return p.distance(v);
  }*/

  //    double ivx, ivy, fvx, fvy, vx, vy;
  //    ivx = v[0]->getX();
  //    ivy = v[0]->getY();
  //    fvx = v[1]->getX();
  //    fvy = v[1]->getY();
  //    vx = v->getX();
  //    vy = v->getY();
  //
  //    if (((ivx == vx) && (ivy == vy)) ||
  //        ((fvx == vx) && (fvy == vy)) ||
  //        ((ivy - vy)*(fvx - vx) == (fvy - vy)*(ivx - vx)))
  //    {
  //        return 0.0;
  //    }
  //
  //    double t = (vx - ivx)/(fvx - ivx);
  //    double y = (ivy + (fvy - ivy)*t);
  //
  //    if (y == vy)
  //    {
  //        if (t < 0.0)
  //        {
  //            return v[0]->distance(v);
  //        }
  //        else if (t > 1.0)
  //        {
  //            return v[1]->distance(v);
  //        }
  //        else
  //        {
  //            return 0.0;
  //        }
  //    }
  //
  //    std::shared_ptr<Edge> e1 = std::make_shared<Edge(0, v[0], v);
  //    std::shared_ptr<Edge> e2 = std::make_shared<Edge(0, v, v[1]);
  //
  //    if (angle(e1) >= M_PI/2.0)
  //    {
  //        return v[0]->distance(v);
  //    }
  //
  //    if (angle(e2) >= M_PI/2.0)
  //    {
  //        return v[1]->distance(v);
  //    }
  //
  //    double l1 = e1->length();
  //    double l2 = e2->length();
  //    double l = length();
  //
  //    if (l1+l2 - l < 0.001)
  //    {
  //        return 0.0;
  //    }
  //
  //    double l1l1 = l1*l1;
  //    double l2l2 = l2*l2;
  //
  //    double m = (l*l - (l2l2 - l1l1))/(2.0*l);
  //
  //    double val = l1l1 - m*m;
  //    double d = val < 0.0 ? 0.0 : sqrt(val);
  //
  //    delete e1;
  //    delete e2;
  //
  //    return d;
}

double Edge::distance(double x, double y) {
  /*static*/ Vertex v;

  v.setPosition(x, y);

  return distance(std::make_shared<Vertex>(v));
}

double Edge::dot(std::shared_ptr<Vertex> v) {
  /*static*/ Vertex vAux;

  vAux.setPosition(v->getX() - this->v[0]->getX(),
                   v->getY() - this->v[0]->getY());

  return vector->dot(std::make_shared<Vertex>(vAux));
}

bool Edge::left(std::shared_ptr<Vertex> v) {
  return left(v->getX(), v->getY());
}

bool Edge::left(double x, double y) {
  /*static*/ Vertex vAux;

  vAux.setPosition(x - this->v[0]->getX(), y - this->v[0]->getY());

  return vAux.right(vector);
}

bool Edge::right(std::shared_ptr<Vertex> v) {
  return right(v->getX(), v->getY());
}

bool Edge::right(double x, double y) {
  /*static*/ Vertex vAux;

  vAux.setPosition(x - this->v[0]->getX(), y - this->v[0]->getY());

  return vAux.left(vector);
}

bool Edge::accordingToNormal(std::shared_ptr<Vertex> v, bool inEdgeTest) {
  if (inEdgeTest) {
    return !right(v);
  }

  return left(v);
}

double Edge::angle(std::shared_ptr<Edge> e) {
  /*static*/ Vertex va, vb, vc;

  if (v[0]->matches(e->v[0])) {
    va = *v[0];
    vb = *v[1];
    vc = *(e->v[1]);
  } else if (v[0]->matches(e->v[1])) {
    va = *v[0];
    vb = *v[1];
    vc = *(e->v[0]);
  } else if (v[1]->matches(e->v[0])) {
    va = *v[1];
    vb = *v[0];
    vc = *(e->v[1]);
  } else if (v[1]->matches(e->v[1])) {
    va = *v[1];
    vb = *v[0];
    vc = *(e->v[0]);
  } else {
    return -1.0;
  }

  vb.setPosition(vb.getX() - va.getX(), vb.getY() - va.getY());
  vc.setPosition(vc.getX() - va.getX(), vc.getY() - va.getY());

  return vb.angle(std::make_shared<Vertex>(vc));
}

double Edge::angle(std::shared_ptr<Vertex> va, std::shared_ptr<Vertex> vb) {
  /*static*/ Edge e;
  /*static*/ double result;

  e.setVertices(va, vb);

  result = angle(std::make_shared<Edge>(e));

  e.setVertices(nullptr, nullptr);

  return result;
}

std::shared_ptr<Vertex> Edge::normal() {
  std::shared_ptr<Vertex> normal = std::make_shared<Vertex>();

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
    // curva 1
    // Ponto p = c->pontoMedio(v[0]->getX(), v[1]->getX());

    t = c->CalculateMidparameterByParamters(v[0]->getX(), v[1]->getX());

    mid = std::make_shared<Vertex>(t, 0.0);

    len = c->CalculateParametricLength(v[0]->getX(), v[1]->getX()) /
          c->GetLength();
  } else if ((v[0]->getX() == 1.0) && (v[0]->getY() < 1.0)) {
    // curva 2
    // Ponto p = c->pontoMedio(v[0]->getY(), v[1]->getY());

    t = c->CalculateMidparameterByParamters(v[0]->getY(), v[1]->getY());

    mid = std::make_shared<Vertex>(1.0, t);

    len = c->CalculateParametricLength(v[0]->getY(), v[1]->getY()) /
          c->GetLength();
  } else if ((v[0]->getY() == 1.0) && (v[0]->getX() > 0.0)) {
    // curva 3
    // Ponto p = c->pontoMedio(v[0]->getX(), v[1]->getX());

    t = c->CalculateMidparameterByParamters(v[1]->getX(), v[0]->getX());

    mid = std::make_shared<Vertex>(t, 1.0);

    len = c->CalculateParametricLength(v[1]->getX(), v[0]->getX()) /
          c->GetLength();
  } else if ((v[0]->getX() == 0.0) && (v[0]->getY() > 0.0)) {
    // curva 4
    // Ponto p = c->pontoMedio(v[0]->getY(), v[1]->getY());

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
