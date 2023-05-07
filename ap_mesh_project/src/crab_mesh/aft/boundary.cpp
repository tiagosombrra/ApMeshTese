#include "../../../include/crab_mesh/aft/boundary.h"

Boundary::Boundary()
    : Shape(), lastVertexId(0), lastEdgeId(0), first(nullptr) {}

Boundary::~Boundary() {}

std::shared_ptr<Edge> Boundary::makeEdge(std::shared_ptr<Vertex> v) {
  auto e = std::make_shared<Edge>(boundary.back(), v, ++lastEdgeId);

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
  auto v = make_shared<Vertex>(x, y, ++lastVertexId);
  if (!first) {
    first = v;
  } else if (!boundary.empty()) {
    auto e = makeEdge(v);
    e->setCurva(c);
    e->makeParamMid();
    edges.push_back(e);
    e->setInBoundary(true);
  }

  boundary.push_back(v);

  return v;
}

std::shared_ptr<Vertex> Boundary::addVertex(long int id, double x, double y) {
  auto v = addVertex(x, y, nullptr);

  if (v) {
    v->setId(id);

    if (id > lastVertexId) {
      lastVertexId = id;
    }
  }

  return v;
}

bool Boundary::close(std::shared_ptr<CurveAdaptiveParametric> c) {
  auto e = std::make_shared<Edge>(boundary.back(), first, ++lastEdgeId);
  e->setCurva(c);
  e->makeParamMid();
  boundary.back()->addAdjacentEdge(e);
  first->addAdjacentEdge(e);
  first = nullptr;
  e->setInBoundary(true);
  edges.push_back(e);

  return true;
}

std::shared_ptr<Edge> Boundary::getEdge(long int id) {
  auto iter = std::find_if(edges.begin(), edges.end(),
                           [id](const auto &e) { return e->getId() == id; });

  return iter != edges.end() ? *iter : nullptr;
}

std::shared_ptr<Vertex> Boundary::getVertex(long int id) {
  auto iter = std::find_if(boundary.begin(), boundary.end(),
                           [id](const auto &v) { return v->getId() == id; });

  return iter != boundary.end() ? *iter : nullptr;
}

void Boundary::getBox(double &minX, double &minY, double &maxX, double &maxY) {
  auto [min_iter, max_iter] = std::minmax_element(
      boundary.begin(), boundary.end(),
      [](const auto &a, const auto &b) { return a->getX() < b->getX(); });

  minX = (*min_iter)->getX();
  maxX = (*max_iter)->getX();

  std::tie(min_iter, max_iter) = std::minmax_element(
      boundary.begin(), boundary.end(),
      [](const auto &a, const auto &b) { return a->getY() < b->getY(); });

  minY = (*min_iter)->getY();
  maxY = (*max_iter)->getY();
}

bool Boundary::belongs(std::shared_ptr<Edge> e) {
  return std::find(edges.begin(), edges.end(), e) != edges.end();
}

bool Boundary::belongs(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2) {
  auto edge_iter = std::find_if(edges.begin(), edges.end(),
                                [v1, v2](const std::shared_ptr<Edge> &edge) {
                                  return edge->equals(v1, v2);
                                });

  return edge_iter != edges.end();
}

string Boundary::getText() {
  string s;

  return s;
}
