#include "../../../include/crab_mesh/aft/vertex.h"

extern double kToleranceAft;

Vertex::Vertex(double x, double y, long int id) : Shape(id) {
  setPosition(x, y);
}

Vertex::~Vertex() { adjacentEdges.clear(); }

void Vertex::setX(double x) { this->x = x; }

void Vertex::setY(double y) { this->y = y; }

void Vertex::setPosition(double x, double y) {
  setX(x);
  setY(y);
}

double Vertex::getX() { return x; }

double Vertex::getY() { return y; }

void Vertex::getPosition(double *x, double *y) {
  *x = this->x;
  *y = this->y;
}

void Vertex::addAdjacentEdge(std::shared_ptr<Edge> e) {
  adjacentEdges.insert(e);
}

void Vertex::removeAdjacentEdge(std::shared_ptr<Edge> e) {
  adjacentEdges.erase(e);
}

EdgeSet Vertex::getAdjacentEdges() { return adjacentEdges; }

void Vertex::sum(std::shared_ptr<Vertex> v) { setPosition(x + v->x, y + v->y); }

void Vertex::scalarMultiplication(double d) { setPosition(x * d, y * d); }

double Vertex::dot(Vertex v) { return (x * v.x + y * v.y); }

double Vertex::cross(Vertex v) { return (x * v.y - y * v.x); }

double Vertex::norm() { return sqrt(x * x + y * y); }

double Vertex::distance(Vertex v) {
  return sqrt(((v.x) - x) * ((v.x) - x) + ((v.y) - y) * ((v.y) - y));
}

double Vertex::distance(std::shared_ptr<Vertex> v) {
  return sqrt(((v->x) - x) * ((v->x) - x) + ((v->y) - y) * ((v->y) - y));
}

double Vertex::angle(Vertex v) { return acos(dot(v) / (norm() * v.norm())); }

double Vertex::pseudoAngle() { return x / norm(); }

double Vertex::pseudoAngle(Vertex v) { return dot(v) / (norm() * v.norm()); }

double Vertex::squarePseudoAngle() {
  if (x >= 0.0) {
    if (y >= 0.0) {
      if (x >= y) {
        return y / x;
      } else {
        return 2.0 - x / y;
      }
    } else {
      if (x >= fabs(y)) {
        return 8.0 + y / x;
      } else {
        return 6.0 - x / y;
      }
    }
  } else {
    if (y >= 0.0) {
      if (fabs(x) >= y) {
        return 4.0 + y / x;
      } else {
        return 2.0 - x / y;
      }
    } else {
      if (fabs(x) >= fabs(y)) {
        return 4.0 + y / x;
      } else {
        return 6.0 - x / y;
      }
    }
  }
}

double Vertex::orientedAngle() {
  Vertex v;
  double ang;

  v.setPosition(1.0, 0.0);

  ang = angle(v);

  return (y >= 0.0) ? ang : 2.0 * M_PI - ang;
}

double Vertex::orientedAngle(std::shared_ptr<Vertex> v) {
  return v->orientedAngle() - orientedAngle();
}

bool Vertex::left(Vertex v) { return (cross(v) > 0.0); }

bool Vertex::right(Vertex v) { return (cross(v) < 0.0); }

double Vertex::surface(Vertex v1, Vertex v2) {
  return fabs(orientedSurface(v1, v2));
}

double Vertex::orientedSurface(Vertex v1, Vertex v2) {
  return (x * v1.y - v1.x * y + v1.x * v2.y - v2.x * v1.y + v2.x * y -
          x * v2.y) /
         2.0;
}

bool Vertex::counterClockWise(Vertex v1, Vertex v2) {
  return orientedSurface(v1, v2) > 0.0;
}

bool Vertex::clockWise(Vertex v1, Vertex v2) {
  return orientedSurface(v1, v2) < 0.0;
}

bool Vertex::equals(double x, double y) {
  return ((this->x > x - kToleranceAft) && (this->x < x + kToleranceAft) &&
          (this->y > y - kToleranceAft) && (this->y < y + kToleranceAft));
}

bool Vertex::matches(std::shared_ptr<Vertex> v) {
  return ((x == v->x) && (y == v->y));
}

string Vertex::getText() {
  ostringstream stream;

  stream << id << " " << x << " " << y;

  string s = stream.str();

  return s;
}
