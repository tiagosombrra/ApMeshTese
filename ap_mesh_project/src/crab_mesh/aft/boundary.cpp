#include "../../../include/crab_mesh/aft/boundary.h"

#include <memory>

Boundary::Boundary() : Shape() {
  lastVertexId = lastEdgeId = 0;

  first = nullptr;
}

Boundary::~Boundary() {
  while (!edges.empty()) {
    std::shared_ptr<Edge> e = edges.front();
    edges.pop_front();

    e->setVertices(nullptr, nullptr);
  }

  while (!boundary.empty()) {
    std::shared_ptr<Vertex> v = boundary.front();
    boundary.pop_front();
  }
}

std::shared_ptr<Edge> Boundary::makeEdge(std::shared_ptr<Vertex> v) {
  std::shared_ptr<Edge> e = nullptr;

  e = std::make_shared<Edge>(boundary.back(), v, ++lastEdgeId);

  boundary.back()->addAdjacentEdge(e);
  v->addAdjacentEdge(e);

  return e;
}

void Boundary::setBoundary(VertexList boundary) { this->boundary = boundary; }

VertexList Boundary::getBoundary() { return boundary; }

void Boundary::setEdges(EdgeList edges) {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    (*iter)->setInBoundary(false);
  }

  this->edges = edges;

  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    (*iter)->setInBoundary(true);
  }
}

EdgeList Boundary::getEdges() { return edges; }

long int Boundary::getLastVertexId() { return lastVertexId; }

long int Boundary::getLastEdgeId() { return lastEdgeId; }

std::shared_ptr<Vertex> Boundary::addVertex(
    double x, double y, std::shared_ptr<CurveAdaptiveParametric> c) {
  std::shared_ptr<Vertex> v = nullptr;

  v = std::make_shared<Vertex>(x, y, ++lastVertexId);

  // #if USE_OPENGL
  //     //figura
  //     //v->setSize(3.0);
  //     //v->setSize(1.0);
  //     v->setColor(0.0, 0.0, 0.0);
  //     //endfigura
  // #endif //#if USE_OPENGL

  if (!first) {
    first = v;
  } else if (!boundary.empty()) {
    std::shared_ptr<Edge> e = makeEdge(v);

    e->setCurva(c);

    e->makeParamMid();

    ////figura
    // #if USE_OPENGL
    //  e->setColor(0.0, 0.0, 0.0);
    // #endif //#if USE_OPENGL
    ////endfigura

    edges.push_back(e);

    e->setInBoundary(true);

    // figura
    // e->setWidth(2.0);
    // endfigura
  }

  boundary.push_back(v);

  return v;
}

std::shared_ptr<Vertex> Boundary::addVertex(long int id, double x, double y) {
  std::shared_ptr<Vertex> v = addVertex(x, y, nullptr);

  if (v) {
    v->setId(id);

    if (id > lastVertexId) {
      lastVertexId = id;
    }
  }

  return v;
}

bool Boundary::close(std::shared_ptr<CurveAdaptiveParametric> c) {
  std::shared_ptr<Edge> e =
      std::make_shared<Edge>(boundary.back(), first, ++lastEdgeId);

  e->setCurva(c);

  e->makeParamMid();

  boundary.back()->addAdjacentEdge(e);
  first->addAdjacentEdge(e);

  first = nullptr;

  ////figura
  // #if USE_OPENGL
  //  e->setColor(0.0, 0.0, 0.0);
  // #endif //#if USE_OPENGL
  ////endfigura

  e->setInBoundary(true);

  edges.push_back(e);

  return true;
}

std::shared_ptr<Edge> Boundary::getEdge(long int id) {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter)->getId() == id) {
      return (*iter);
    }
  }

  return nullptr;
}

std::shared_ptr<Vertex> Boundary::getVertex(long int id) {
  for (VertexList::iterator iter = boundary.begin(); iter != boundary.end();
       iter++) {
    if ((*iter)->getId() == id) {
      return (*iter);
    }
  }

  return nullptr;
}

void Boundary::getBox(double *minX, double *minY, double *maxX, double *maxY) {
  *minX = boundary.front()->getX();
  *maxX = boundary.front()->getX();

  *minY = boundary.front()->getY();
  *maxY = boundary.front()->getY();

  for (VertexList::iterator iter = ++boundary.begin(); iter != boundary.end();
       iter++) {
    if ((*iter)->getX() < *minX) {
      *minX = (*iter)->getX();
    }

    if ((*iter)->getY() < *minY) {
      *minY = (*iter)->getY();
    }

    if ((*iter)->getX() > *maxX) {
      *maxX = (*iter)->getX();
    }

    if ((*iter)->getY() > *maxY) {
      *maxY = (*iter)->getY();
    }
  }
}

bool Boundary::belongs(std::shared_ptr<Edge> e) {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter) == e) {
      return true;
    }
  }

  return false;
}

bool Boundary::belongs(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter)->equals(v1, v2)) {
      return true;
    }
  }

  return false;
}

string Boundary::getText() {
  string s;

  return s;
}
