#include "../../../include/crab_mesh/aft/quadtree_cell.h"

extern double kToleranceAft;

QuadtreeCell::QuadtreeCell(long int id, Quadtree *tree,
                           std::shared_ptr<Vertex> min,
                           std::shared_ptr<Vertex> max, QuadtreeCell *parent,
                           long int level)
    : Shape(id) {
  setQuadtree(tree);
  setBox(min, max);
  setParent(parent);
  setLevel(level);

  children[QUAD_BOTTOM_LEFT] = children[QUAD_BOTTOM_RIGHT] =
      children[QUAD_TOP_RIGHT] = children[QUAD_TOP_LEFT] = nullptr;

  mids.clear();

  for (int i = 0; i < 8; i++) {
    v[i] = nullptr;
    e[i] = nullptr;
  }
}

QuadtreeCell::~QuadtreeCell() {
  edges.clear();

  neighbors[QUAD_BOTTOM].clear();
  neighbors[QUAD_RIGHT].clear();
  neighbors[QUAD_TOP].clear();
  neighbors[QUAD_LEFT].clear();
}

std::vector<std::shared_ptr<Vertex>> QuadtreeCell::makeMids() {
  std::vector<std::shared_ptr<Vertex>> m;

  if (min && max) {
    /*
     *        -----mids[2]---max
     *       |                |
     *    mids[3]  mids[4]  mids[1]
     *       |                |
     *      min----mids[0]----
     *
     */

    double x = (min->getX() + max->getX()) / 2.0;
    double y = (min->getY() + max->getY()) / 2.0;

    m.resize(5);
    m[QUAD_BOTTOM] = std::make_shared<Vertex>(x, min->getY());
    m[QUAD_RIGHT] = std::make_shared<Vertex>(max->getX(), y);
    m[QUAD_TOP] = std::make_shared<Vertex>(x, max->getY());
    m[QUAD_LEFT] = std::make_shared<Vertex>(min->getX(), y);
    m[QUAD_MID] = std::make_shared<Vertex>(x, y);
  }

  return m;
}

void QuadtreeCell::subdivide(bool setChildrenEdges) {
  /*
   *        -------------------
   *       | chil[3] | chil[2] |
   *       |---------|---------|
   *       | chil[0] | chil[1] |
   *        -------------------
   *
   */

  mids = makeMids();

  children[QUAD_BOTTOM_LEFT] =
      std::make_shared<QuadtreeCell>(tree->getCellId(), tree, min,
                                     mids[QUAD_MID], this, level + 1)
          .get();
  children[QUAD_BOTTOM_RIGHT] =
      std::make_shared<QuadtreeCell>(tree->getCellId(), tree, mids[QUAD_BOTTOM],
                                     mids[QUAD_RIGHT], this, level + 1)
          .get();
  children[QUAD_TOP_RIGHT] =
      std::make_shared<QuadtreeCell>(tree->getCellId(), tree, mids[QUAD_MID],
                                     max, this, level + 1)
          .get();
  children[QUAD_TOP_LEFT] =
      std::make_shared<QuadtreeCell>(tree->getCellId(), tree, mids[QUAD_LEFT],
                                     mids[QUAD_TOP], this, level + 1)
          .get();

  setChildrenNeighbors();

  if (setChildrenEdges) {
    this->setChildrenEdges();
  }

  tree->addLeaves(this);

  neighbors[QUAD_BOTTOM].clear();
  neighbors[QUAD_RIGHT].clear();
  neighbors[QUAD_TOP].clear();
  neighbors[QUAD_LEFT].clear();
}

void QuadtreeCell::setChildrenNeighbors() {
  children[QUAD_BOTTOM_LEFT]->neighbors[QUAD_RIGHT].push_back(
      children[QUAD_BOTTOM_RIGHT]);
  children[QUAD_BOTTOM_LEFT]->neighbors[QUAD_TOP].push_back(
      children[QUAD_TOP_LEFT]);

  children[QUAD_BOTTOM_RIGHT]->neighbors[QUAD_TOP].push_back(
      children[QUAD_TOP_RIGHT]);
  children[QUAD_BOTTOM_RIGHT]->neighbors[QUAD_LEFT].push_back(
      children[QUAD_BOTTOM_LEFT]);

  children[QUAD_TOP_RIGHT]->neighbors[QUAD_BOTTOM].push_back(
      children[QUAD_BOTTOM_RIGHT]);
  children[QUAD_TOP_RIGHT]->neighbors[QUAD_LEFT].push_back(
      children[QUAD_TOP_LEFT]);

  children[QUAD_TOP_LEFT]->neighbors[QUAD_BOTTOM].push_back(
      children[QUAD_BOTTOM_LEFT]);
  children[QUAD_TOP_LEFT]->neighbors[QUAD_RIGHT].push_back(
      children[QUAD_TOP_RIGHT]);

  for (QuadtreeCellList::iterator iter = neighbors[QUAD_BOTTOM].begin();
       iter != neighbors[QUAD_BOTTOM].end(); iter++) {
    auto q = (*iter);

    q->neighbors[QUAD_TOP].remove(this);

    if (q->min->getX() < children[QUAD_BOTTOM_LEFT]->max->getX()) {
      children[QUAD_BOTTOM_LEFT]->neighbors[QUAD_BOTTOM].push_back(q);
      q->neighbors[QUAD_TOP].push_back(children[QUAD_BOTTOM_LEFT]);
    }

    if (q->max->getX() > children[QUAD_BOTTOM_RIGHT]->min->getX()) {
      children[QUAD_BOTTOM_RIGHT]->neighbors[QUAD_BOTTOM].push_back(q);
      q->neighbors[QUAD_TOP].push_back(children[QUAD_BOTTOM_RIGHT]);
    }
  }

  for (QuadtreeCellList::iterator iter = neighbors[QUAD_RIGHT].begin();
       iter != neighbors[QUAD_RIGHT].end(); iter++) {
    auto q = (*iter);

    q->neighbors[QUAD_LEFT].remove(this);

    if (q->min->getY() < children[QUAD_BOTTOM_RIGHT]->max->getY()) {
      children[QUAD_BOTTOM_RIGHT]->neighbors[QUAD_RIGHT].push_back(q);
      q->neighbors[QUAD_LEFT].push_back(children[QUAD_BOTTOM_RIGHT]);
    }

    if (q->max->getY() > children[QUAD_TOP_RIGHT]->min->getY()) {
      children[QUAD_TOP_RIGHT]->neighbors[QUAD_RIGHT].push_back(q);
      q->neighbors[QUAD_LEFT].push_back(children[QUAD_TOP_RIGHT]);
    }
  }

  for (QuadtreeCellList::iterator iter = neighbors[QUAD_TOP].begin();
       iter != neighbors[QUAD_TOP].end(); iter++) {
    auto q = (*iter);

    q->neighbors[QUAD_BOTTOM].remove(this);

    if (q->max->getX() > children[QUAD_TOP_RIGHT]->min->getX()) {
      children[QUAD_TOP_RIGHT]->neighbors[QUAD_TOP].push_back(q);
      q->neighbors[QUAD_BOTTOM].push_back(children[QUAD_TOP_RIGHT]);
    }

    if (q->min->getX() < children[QUAD_TOP_LEFT]->max->getX()) {
      children[QUAD_TOP_LEFT]->neighbors[QUAD_TOP].push_back(q);
      q->neighbors[QUAD_BOTTOM].push_back(children[QUAD_TOP_LEFT]);
    }
  }

  for (QuadtreeCellList::iterator iter = neighbors[QUAD_LEFT].begin();
       iter != neighbors[QUAD_LEFT].end(); iter++) {
    auto q = (*iter);

    q->neighbors[QUAD_RIGHT].remove(this);

    if (q->max->getY() > children[QUAD_TOP_LEFT]->min->getY()) {
      children[QUAD_TOP_LEFT]->neighbors[QUAD_LEFT].push_back(q);
      q->neighbors[QUAD_RIGHT].push_back(children[QUAD_TOP_LEFT]);
    }

    if (q->min->getY() < children[QUAD_BOTTOM_LEFT]->max->getY()) {
      children[QUAD_BOTTOM_LEFT]->neighbors[QUAD_LEFT].push_back(q);
      q->neighbors[QUAD_RIGHT].push_back(children[QUAD_BOTTOM_LEFT]);
    }
  }
}

void QuadtreeCell::setChildrenEdges() {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    auto e = (*iter);
    tree->findCell(e);
  }
}

std::shared_ptr<Edge> QuadtreeCell::makeE(int i) {
  std::pair<int, int> v1v2[] = {{1, 0}, {2, 1}, {3, 2}, {4, 3},
                                {5, 4}, {6, 5}, {7, 6}, {0, 7}};

  int index = i % 8;
  int v1 = v[v1v2[index].first - 1] ? v1v2[index].first : v1v2[index].second;
  int v2 = v1v2[index].second;

  return makeE(v1, v2);
}

std::shared_ptr<Edge> QuadtreeCell::makeE(int i, int j) {
  return makeE(v[i], v[j]);
}

std::shared_ptr<Edge> QuadtreeCell::makeE(std::shared_ptr<Vertex> v1,
                                          std::shared_ptr<Vertex> v2) {
  auto e = std::make_shared<Edge>(v1, v2, tree->edgeId());

  v1->addAdjacentEdge(e);
  v2->addAdjacentEdge(e);

  e->setFree(false);

  return e;
}

void QuadtreeCell::makeMeshType1() {
  //	    v[6] -------------e[4]------------- v[4]
  //        | \                                 |
  //        |   \                               |
  //        |     \                             |
  //        |       \                           |
  //        |         \                         |
  //        |           \                       |
  //        |             \                     |
  //      e[6]              \                 e[2]
  //        |                 \                 |
  //        |                   \               |
  //        |                     \             |
  //        |                       \           |
  //        |                         \         |
  //        |                           \       |
  //        |                             \     |
  //	    v[0] ------------ e[0] ------------ v[2]

  int numEdges = 1;
  int numFaces = 2;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);

  edges[0] = makeE(2, 6);

  faces[0] = std::make_shared<Face>(v[0], v[2], v[6], tree->faceId());
  faces[1] = std::make_shared<Face>(v[4], v[6], v[2], tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }
}

void QuadtreeCell::makeMeshType2(int c) {
  //	v[6] ------------ e[4] -------- --- v[4]
  //        |  \                                |
  //        |      \                            |
  //        |          \                        |
  //        |              \                  e[3]
  //        |                  \                |
  //        |                      \            |
  //        |                          \        |
  //      e[6]                             *  v[3]     c == QUAD_R
  //        |                          /        |
  //        |                      /            |
  //        |                  /                |
  //        |              /                  e[2]
  //        |          /                        |
  //        |      /                            |
  //        |  /                                |
  //		v[0] ------------ e[0] ------------ v[2]

  int numEdges = 2;
  int numFaces = 3;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);

  int base = 0;

  switch (c) {
    case QUAD_B:
      base = 1;
      break;
    case QUAD_R:
      base = 3;
      break;
    case QUAD_T:
      base = 5;
      break;
    case QUAD_L:
      base = 7;
      break;
  }

  edges[0] = makeE(base, (base + 5) % 8);
  edges[1] = makeE(base, (base + 3) % 8);

  faces[0] = std::make_shared<Face>(v[(base + 7) % 8], v[base],
                                    v[(base + 5) % 8], tree->faceId());
  faces[1] = std::make_shared<Face>(v[(base + 5) % 8], v[base],
                                    v[(base + 3) % 8], tree->faceId());
  faces[2] = std::make_shared<Face>(v[(base + 3) % 8], v[base],
                                    v[(base + 1) % 8], tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }
}

void QuadtreeCell::makeMeshType3(int c) {
  //                    c == QUAD_T
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        | \              |             /    |
  //        |   \            |           /      |
  //        |     \          |         /        |
  //        |       \        |       /          |
  //        |         \      |     /            |
  //        |           \    |   /              |
  //        |             \  | /                |
  //      e[6]               *                e[2]
  //        |             /  | \                |
  //        |           /    |   \              |
  //        |         /      |     \            |
  //        |       /        |       \          |
  //        |     /          |         \        |
  //        |   /            |           \      |
  //        | /              |             \    |
  //	    v[0] --- e[0] --- v[1] --- e[1] --- v[2]
  //                    c == QUAD_B

  int numEdges = 6;
  int numFaces = 6;
  int numVertices = 1;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);
  std::vector<std::shared_ptr<Vertex>> vertices(numVertices);

  int base = 0;

  switch (c) {
    case QUAD_B:
    case QUAD_T:
      base = 5;
      break;
    case QUAD_R:
    case QUAD_L:
      base = 7;
      break;
  }

  vertices[0] = std::make_shared<Vertex>(0.5 * (min->getX() + max->getX()),
                                         0.5 * (min->getY() + max->getY()),
                                         tree->vertexId());

  edges[0] = makeE(v[base], vertices[0]);
  edges[1] = makeE(v[(base + 1) % 8], vertices[0]);
  edges[2] = makeE(v[(base + 3) % 8], vertices[0]);
  edges[3] = makeE(v[(base + 4) % 8], vertices[0]);
  edges[4] = makeE(v[(base + 5) % 8], vertices[0]);
  edges[5] = makeE(v[(base + 7) % 8], vertices[0]);

  faces[0] = std::make_shared<Face>(v[base], v[(base + 1) % 8], vertices[0],
                                    tree->faceId());
  faces[1] = std::make_shared<Face>(v[(base + 1) % 8], v[(base + 3) % 8],
                                    vertices[0], tree->faceId());
  faces[2] = std::make_shared<Face>(v[(base + 3) % 8], v[(base + 4) % 8],
                                    vertices[0], tree->faceId());
  faces[3] = std::make_shared<Face>(v[(base + 4) % 8], v[(base + 5) % 8],
                                    vertices[0], tree->faceId());
  faces[4] = std::make_shared<Face>(v[(base + 5) % 8], v[(base + 7) % 8],
                                    vertices[0], tree->faceId());
  faces[5] = std::make_shared<Face>(v[(base + 7) % 8], v[base], vertices[0],
                                    tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }

  for (int i = 0; i < numVertices; i++) {
    tree->add(vertices[i]);
  }
}

void QuadtreeCell::makeMeshType4(int c) {
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        | \             |  \                |
  //        |   \           |    \              |
  //        |     \         |      \            |
  //        |       \       |        \        e[3]
  //        |         \     |          \        |
  //        |           \   |            \      |
  //        |             \ |              \    |
  //      e[6]              * --------------  v[3]   c == QUAD_R
  //        |             /   \                 |
  //        |           /       \               |
  //        |         /           \             |
  //        |       /               \         e[2]
  //        |     /                   \         |
  //        |   /                       \       |
  //        | /                           \     |
  //		v[0] ------------ e[0] ------------ v[2]

  int numEdges = 6;
  int numFaces = 6;
  int numVertices = 1;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);
  std::vector<std::shared_ptr<Vertex>> vertices(numVertices);

  int base = 0;

  switch (c) {
    case QUAD_B:
      base = 1;
      break;
    case QUAD_R:
      base = 3;
      break;
    case QUAD_T:
      base = 5;
      break;
    case QUAD_L:
      base = 7;
      break;
  }

  vertices[0] = std::make_shared<Vertex>(0.5 * (min->getX() + max->getX()),
                                         0.5 * (min->getY() + max->getY()),
                                         tree->vertexId());

  edges[0] = makeE(base, (base + 2) % 8);
  edges[1] = makeE(v[base], vertices[0]);
  edges[2] = makeE(v[(base + 2) % 8], vertices[0]);
  edges[3] = makeE(v[(base + 3) % 8], vertices[0]);
  edges[4] = makeE(v[(base + 5) % 8], vertices[0]);
  edges[5] = makeE(v[(base + 7) % 8], vertices[0]);

  faces[0] = std::make_shared<Face>(v[base], v[(base + 1) % 8],
                                    v[(base + 2) % 8], tree->faceId());
  faces[1] = std::make_shared<Face>(v[base], v[(base + 2) % 8], vertices[0],
                                    tree->faceId());
  faces[2] = std::make_shared<Face>(v[(base + 2) % 8], v[(base + 3) % 8],
                                    vertices[0], tree->faceId());
  faces[3] = std::make_shared<Face>(v[(base + 3) % 8], v[(base + 5) % 8],
                                    vertices[0], tree->faceId());
  faces[4] = std::make_shared<Face>(v[(base + 5) % 8], v[(base + 7) % 8],
                                    vertices[0], tree->faceId());
  faces[5] = std::make_shared<Face>(v[(base + 7) % 8], v[base], vertices[0],
                                    tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }

  for (int i = 0; i < numVertices; i++) {
    tree->add(vertices[i]);
  }
}

void QuadtreeCell::makeMeshType5(int c) {
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        |             /  | \                |
  //        |           /    |  \               |
  //        |         /      |   \              |
  //      e[6]      /        |    \             |
  //        |     /          |     \            |
  //        |   /            |      \           |
  //        | /              |       \          |
  //      v[7]               |        \       e[2]  c == QUAD_R
  //        | \              |         \        |
  //        |   \            |          \       |
  //        |     \          |           \      |
  //      e[7]      \        |            \     |
  //        |         \      |             \    |
  //        |           \    |              \   |
  //        |             \  |               \  |
  //		v[0] --- e[0] --- v[1] --- e[1] --- v[2]

  int numEdges = 4;
  int numFaces = 5;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);

  int base = 0;

  switch (c) {
    case QUAD_B:
      base = 3;
      break;
    case QUAD_R:
      base = 5;
      break;
    case QUAD_T:
      base = 7;
      break;
    case QUAD_L:
      base = 1;
      break;
  }

  edges[0] = makeE(base, (base + 2) % 8);
  edges[1] = makeE(base, (base + 4) % 8);
  edges[2] = makeE(base, (base + 5) % 8);
  edges[3] = makeE((base + 2) % 8, (base + 4) % 8);

  faces[0] = std::make_shared<Face>(v[(base + 2) % 8], v[base],
                                    v[(base + 1) % 8], tree->faceId());
  faces[1] = std::make_shared<Face>(v[(base + 4) % 8], v[base],
                                    v[(base + 2) % 8], tree->faceId());
  faces[2] = std::make_shared<Face>(v[(base + 5) % 8], v[base],
                                    v[(base + 4) % 8], tree->faceId());
  faces[3] = std::make_shared<Face>(v[(base + 7) % 8], v[base],
                                    v[(base + 5) % 8], tree->faceId());
  faces[4] = std::make_shared<Face>(v[(base + 3) % 8], v[(base + 4) % 8],
                                    v[(base + 2) % 8], tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }
}

void QuadtreeCell::makeMeshType6() {
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        |              / | \                |
  //        |            /   |   \              |
  //        |          /     |     \            |
  //      e[6]       /       |       \        e[3]
  //        |      /         |         \        |
  //        |    /           |           \      |
  //        |  /             |             \    |
  //      v[7]               |                v[3]
  //        |  \             |             /    |
  //        |    \           |           /      |
  //        |      \         |         /        |
  //      e[7]       \       |       /        e[2]
  //        |          \     |     /            |
  //        |            \   |   /              |
  //        |              \ | /                |
  //		v[0] --- e[0] --- v[1] --- e[1] --- v[2]

  int numEdges = 5;
  int numFaces = 6;

  std::vector<std::shared_ptr<Edge>> edges(numEdges);
  std::vector<std::shared_ptr<Face>> faces(numFaces);

  edges[0] = makeE(1, 5);
  edges[1] = makeE(1, 3);
  edges[2] = makeE(3, 5);
  edges[3] = makeE(1, 7);
  edges[4] = makeE(7, 5);

  faces[0] = std::make_shared<Face>(v[0], v[1], v[7], tree->faceId());
  faces[1] = std::make_shared<Face>(v[2], v[3], v[1], tree->faceId());
  faces[2] = std::make_shared<Face>(v[4], v[5], v[3], tree->faceId());
  faces[3] = std::make_shared<Face>(v[6], v[7], v[5], tree->faceId());
  faces[4] = std::make_shared<Face>(v[1], v[3], v[5], tree->faceId());
  faces[5] = std::make_shared<Face>(v[1], v[5], v[7], tree->faceId());

  for (int i = 0; i < numEdges; i++) {
    tree->add(edges[i]);

    edges[i]->setCell(this);
  }

  for (int i = 0; i < numFaces; i++) {
    tree->add(faces[i]);
  }
}

void QuadtreeCell::setQuadtree(Quadtree *tree) { this->tree = tree; }

void QuadtreeCell::setLevel(long int level) { this->level = level; }

long int QuadtreeCell::getLevel() { return level; }

double QuadtreeCell::getSize() { return size; }

void QuadtreeCell::setParent(QuadtreeCell *parent) { this->parent = parent; }

QuadtreeCell *QuadtreeCell::getParent() { return parent; }

void QuadtreeCell::setChild(int position, QuadtreeCell *child) {
  if ((position >= 0) && (position <= 3)) {
    children[position] = child;
  }
}

QuadtreeCell *QuadtreeCell::getChild(int position) {
  return ((position >= 0) && (position <= 3)) ? children[position] : nullptr;
}

void QuadtreeCell::setBox(std::shared_ptr<Vertex> min,
                          std::shared_ptr<Vertex> max) {
  double spanX, spanY;

  this->min = min;
  this->max = max;

  spanX = max->getX() - min->getX();
  spanY = max->getY() - min->getY();

  size = (spanX > spanY) ? spanX : spanY;
}

void QuadtreeCell::setNeighbors(int direction, QuadtreeCellList neighbors) {
  if ((direction < QUAD_BOTTOM) || (direction > QUAD_LEFT)) {
    return;
  }

  this->neighbors[direction] = neighbors;
}

QuadtreeCellList QuadtreeCell::getNeighbors(int direction) {
  QuadtreeCellList l;

  return (((direction < 0) || (direction > 3)) ? l : neighbors[direction]);
}

int QuadtreeCell::getNumCells() {
  int numCells = 1;

  if (subdivided()) {
    numCells += children[QUAD_BOTTOM_LEFT]->getNumCells();
    numCells += children[QUAD_BOTTOM_RIGHT]->getNumCells();
    numCells += children[QUAD_TOP_RIGHT]->getNumCells();
    numCells += children[QUAD_TOP_LEFT]->getNumCells();
  }

  return numCells;
}

VertexList QuadtreeCell::getVertices() {
  VertexList vertices;
  std::unordered_set<std::shared_ptr<Vertex>> vertexSet;

  for (auto &edge : edges) {
    auto v1 = edge->getV1();
    auto v2 = edge->getV2();

    if (vertexSet.find(v1) == vertexSet.end()) {
      vertices.push_back(v1);
      vertexSet.insert(v1);
    }

    if (vertexSet.find(v2) == vertexSet.end()) {
      vertices.push_back(v2);
      vertexSet.insert(v2);
    }
  }

  return vertices;
}

EdgeList QuadtreeCell::getEdges() { return edges; }

bool QuadtreeCell::shouldSubdivide(std::shared_ptr<Edge> e) {
  return (size - e->getLen() * tree->getFactor() >= kToleranceAft);
}

bool QuadtreeCell::shouldSubdivide(std::shared_ptr<Face> f) {
  // Subdivisão de acordo com as curvaturas dos vértices de um elemento
  if ((f->getId() == 2) || (f->getId() == 4)) {
    // cout << "TRIANGULO " << f->getId() << endl;
    // cout << "size = " << size << endl;
    // cout << "f->h = " << f->h << endl;
    // cout << "size - f->h*tree->getFactor() = " << size -
    // f->h*tree->getFactor() << endl; cout << "deve subdividir = " << (size -
    // f->h*tree->getFactor() >= Shape::tolerance) << endl;
  }
  return (size - f->h * tree->getFactor() >= kToleranceAft);
}

void QuadtreeCell::addEdge(std::shared_ptr<Edge> e) {
  bool existed = false;

  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter)->equals(e)) {
      existed = true;
      break;
    }
  }

  if (!existed) {
    edges.push_back(e);
  }
}

void QuadtreeCell::removeEdge(std::shared_ptr<Edge> e) {
  edges.remove(e);

  if (parent) {
    parent->removeEdge(e);
  }
}

void QuadtreeCell::clearEdges() { edges.clear(); }

QuadtreeCell *QuadtreeCell::findCell(std::shared_ptr<Edge> e) {
  if (out(e->getMid())) {
    return nullptr;
  }

  if (subdivided()) {
    auto q = children[QUAD_BOTTOM_LEFT]->findCell(e);

    if (!q) {
      q = children[QUAD_BOTTOM_RIGHT]->findCell(e);
    }

    if (!q) {
      q = children[QUAD_TOP_RIGHT]->findCell(e);
    }

    if (!q) {
      q = children[QUAD_TOP_LEFT]->findCell(e);
    }

    return q;
  }

  return this;
}

QuadtreeCell *QuadtreeCell::findCell(std::shared_ptr<Face> f) {
  if (out(f->getMid())) {
    return NULL;
  }

  if (subdivided()) {
    auto q = children[QUAD_BOTTOM_LEFT]->findCell(f);

    if (!q) {
      q = children[QUAD_BOTTOM_RIGHT]->findCell(f);
    }

    if (!q) {
      q = children[QUAD_TOP_RIGHT]->findCell(f);
    }

    if (!q) {
      q = children[QUAD_TOP_LEFT]->findCell(f);
    }

    return q;
  }

  return this;
}

bool QuadtreeCell::in(std::shared_ptr<Vertex> v) {
  return (v->getX() > min->getX() && v->getX() < max->getX() &&
          v->getY() > min->getY() && v->getY() < max->getY());
}

bool QuadtreeCell::on(std::shared_ptr<Vertex> v) {
  return (!in(v) && !(out(v)));
}

bool QuadtreeCell::out(std::shared_ptr<Vertex> v) {
  return ((v->getX() <= min->getX() - kToleranceAft) ||
          (v->getX() >= max->getX() + kToleranceAft) ||
          (v->getY() <= min->getY() - kToleranceAft) ||
          (v->getY() >= max->getY() + kToleranceAft));
}

double QuadtreeCell::height() { return size; }

double QuadtreeCell::surface() { return size * size; }

std::shared_ptr<Vertex> QuadtreeCell::getMin() { return min; }

std::shared_ptr<Vertex> QuadtreeCell::getMax() { return max; }

bool QuadtreeCell::subdivide(std::shared_ptr<Edge> e) {
  if (out(e->getMid())) {
    return false;
  }

  if (!shouldSubdivide(e)) {
    return false;
  }

  if (!subdivided()) {
    subdivide();
  }

  for (int i = 0; i < 4; i++) {
    children[i]->subdivide(e);
  }

  return true;
}

bool QuadtreeCell::subdivide(std::shared_ptr<Face> f) {
  if (f->getId() == 4) {
    // cout << " ****** TRIANGULO 4 *******" << endl;
  }

  if (out(f->getMid())) {
    return false;
  }

  if (!shouldSubdivide(f)) {
    return false;
  }

  if (f->getId() == 4) {
    // cout << "SUBDIVIDE" << endl;
  }

  if (!subdivided()) {
    subdivide();
  }

  for (int i = 0; i < 4; i++) {
    children[i]->subdivide(f);
  }

  return true;
}

void QuadtreeCell::subdivideToLevel(unsigned long int level) {
  if ((parent) && (this->level <= level) && (edges.empty())) {
    EdgeList pEdges = parent->edges;

    for (EdgeList::iterator iter = pEdges.begin(); iter != pEdges.end();
         iter++) {
      tree->findCell((*iter));
    }
  }

  if (this->level < level) {
    if (!subdivided()) {
      subdivide();
    }

    if (subdivided()) {
      children[QUAD_BOTTOM_LEFT]->subdivideToLevel(level);
      children[QUAD_BOTTOM_RIGHT]->subdivideToLevel(level);
      children[QUAD_TOP_RIGHT]->subdivideToLevel(level);
      children[QUAD_TOP_LEFT]->subdivideToLevel(level);
    }
  }
}

bool QuadtreeCell::subdivideAccordingToNeighbors() {
  if ((neighbors[QUAD_BOTTOM].size() > 2) ||
      (neighbors[QUAD_RIGHT].size() > 2) || (neighbors[QUAD_TOP].size() > 2) ||
      (neighbors[QUAD_LEFT].size() > 2)) {
    subdivide();

    return true;
  }

  return false;
}

bool QuadtreeCell::subdivided() { return ((bool)children[QUAD_BOTTOM_LEFT]); }

void QuadtreeCell::sortNeighbors() {
  if (subdivided()) {
    for (int i = 0; i < 4; i++) {
      children[i]->sortNeighbors();
    }

    return;
  }

  for (int i = 0; i < 4; i++) {
    if (neighbors[i].size() > 1) {
      auto f = neighbors[i].front();
      auto b = neighbors[i].back();

      bool change = false;

      switch (i) {
        case QUAD_B:
          change = b->getMin()->getX() < f->getMin()->getX();
          break;
        case QUAD_R:
          change = b->getMin()->getY() < f->getMin()->getY();
          break;
        case QUAD_T:
          change = f->getMin()->getX() < b->getMin()->getX();
          break;
        case QUAD_L:
          change = f->getMin()->getY() < b->getMin()->getY();
          break;
      }

      if (change) {
        neighbors[i].clear();

        neighbors[i].push_back(b);
        neighbors[i].push_back(f);
      }
    }
  }
}

bool QuadtreeCell::inBoundary() {
  return (neighbors[QUAD_B].empty() || neighbors[QUAD_R].empty() ||
          neighbors[QUAD_T].empty() || neighbors[QUAD_L].empty());
}

void QuadtreeCell::makeOuterVertices() {
  if (subdivided()) {
    for (int i = 0; i < 4; i++) {
      children[i]->makeOuterVertices();
    }

    return;
  }

  if (inBoundary()) {
    return;
  }

  if (!v[0]) {
    v[0] = std::make_shared<Vertex>(min->getX(), min->getY(), tree->vertexId());

    if ((neighbors[QUAD_L].size() == 2) ||
        (neighbors[QUAD_L].back()->level == level) ||
        (neighbors[QUAD_L].back()->neighbors[QUAD_R].front() == this)) {
      neighbors[QUAD_L].back()->v[2] = v[0];
    } else {
      neighbors[QUAD_L].back()->v[3] = v[0];
    }

    if ((neighbors[QUAD_B].size() == 2) ||
        (neighbors[QUAD_B].front()->level == level) ||
        (neighbors[QUAD_B].front()->neighbors[QUAD_T].back() == this)) {
      neighbors[QUAD_B].front()->v[6] = v[0];
    } else {
      neighbors[QUAD_B].front()->v[5] = v[0];
    }

    if (neighbors[QUAD_B].front()->neighbors[QUAD_L].front() ==
        neighbors[QUAD_L].back()->neighbors[QUAD_B].back()) {
      neighbors[QUAD_B].front()->neighbors[QUAD_L].front()->v[4] = v[0];
    }

    tree->add(v[0]);
  }

  if ((!v[1]) && (neighbors[QUAD_B].size() > 1)) {
    v[1] = std::make_shared<Vertex>(0.5 * (min->getX() + max->getX()),
                                    min->getY(), tree->vertexId());

    neighbors[QUAD_B].front()->v[4] = v[1];
    neighbors[QUAD_B].back()->v[6] = v[1];

    tree->add(v[1]);
  }

  if (!v[2]) {
    v[2] = std::make_shared<Vertex>(max->getX(), min->getY(), tree->vertexId());

    if ((neighbors[QUAD_B].size() == 2) ||
        (neighbors[QUAD_B].back()->level == level) ||
        (neighbors[QUAD_B].back()->neighbors[QUAD_T].front() == this)) {
      neighbors[QUAD_B].back()->v[4] = v[2];
    } else {
      neighbors[QUAD_B].back()->v[5] = v[2];
    }

    if ((neighbors[QUAD_R].size() == 2) ||
        (neighbors[QUAD_R].front()->level == level) ||
        (neighbors[QUAD_R].front()->neighbors[QUAD_L].back() == this)) {
      neighbors[QUAD_R].front()->v[0] = v[2];
    } else {
      neighbors[QUAD_R].front()->v[7] = v[2];
    }

    if (neighbors[QUAD_R].front()->neighbors[QUAD_B].front() ==
        neighbors[QUAD_B].back()->neighbors[QUAD_R].back()) {
      neighbors[QUAD_R].front()->neighbors[QUAD_B].front()->v[6] = v[2];
    }

    tree->add(v[2]);
  }

  if ((!v[3]) && (neighbors[QUAD_R].size() > 1)) {
    v[3] = std::make_shared<Vertex>(
        max->getX(), 0.5 * (min->getY() + max->getY()), tree->vertexId());

    neighbors[QUAD_R].front()->v[6] = v[3];
    neighbors[QUAD_R].back()->v[0] = v[3];

    tree->add(v[3]);
  }

  if (!v[4]) {
    v[4] = std::make_shared<Vertex>(max->getX(), max->getY(), tree->vertexId());

    if ((neighbors[QUAD_R].size() == 2) ||
        (neighbors[QUAD_R].back()->level == level) ||
        (neighbors[QUAD_R].back()->neighbors[QUAD_L].front() == this)) {
      neighbors[QUAD_R].back()->v[6] = v[4];
    } else {
      neighbors[QUAD_R].back()->v[7] = v[4];
    }

    if ((neighbors[QUAD_T].size() == 2) ||
        (neighbors[QUAD_T].front()->level == level) ||
        (neighbors[QUAD_T].front()->neighbors[QUAD_B].back() == this)) {
      neighbors[QUAD_T].front()->v[2] = v[4];
    } else {
      neighbors[QUAD_T].front()->v[1] = v[4];
    }

    if (neighbors[QUAD_T].front()->neighbors[QUAD_R].front() ==
        neighbors[QUAD_R].back()->neighbors[QUAD_T].back()) {
      neighbors[QUAD_T].front()->neighbors[QUAD_R].front()->v[0] = v[4];
    }

    tree->add(v[4]);
  }

  if ((!v[5]) && (neighbors[QUAD_T].size() > 1)) {
    v[5] = std::make_shared<Vertex>(0.5 * (min->getX() + max->getX()),
                                    max->getY(), tree->vertexId());

    neighbors[QUAD_T].front()->v[0] = v[5];
    neighbors[QUAD_T].back()->v[2] = v[5];

    tree->add(v[5]);
  }

  if (!v[6]) {
    v[6] = std::make_shared<Vertex>(min->getX(), max->getY(), tree->vertexId());

    if ((neighbors[QUAD_T].size() == 2) ||
        (neighbors[QUAD_T].back()->level == level) ||
        (neighbors[QUAD_T].back()->neighbors[QUAD_B].front() == this)) {
      neighbors[QUAD_T].back()->v[0] = v[6];
    } else {
      neighbors[QUAD_T].back()->v[1] = v[6];
    }

    if ((neighbors[QUAD_L].size() == 2) ||
        (neighbors[QUAD_L].front()->level == level) ||
        (neighbors[QUAD_L].front()->neighbors[QUAD_R].back() == this)) {
      neighbors[QUAD_L].front()->v[4] = v[6];
    } else {
      neighbors[QUAD_L].front()->v[3] = v[6];
    }

    if (neighbors[QUAD_L].front()->neighbors[QUAD_T].front() ==
        neighbors[QUAD_T].back()->neighbors[QUAD_L].back()) {
      neighbors[QUAD_L].front()->neighbors[QUAD_T].front()->v[2] = v[6];
    }

    tree->add(v[6]);
  }

  if ((!v[7]) && (neighbors[QUAD_L].size() > 1)) {
    v[7] = std::make_shared<Vertex>(
        min->getX(), 0.5 * (min->getY() + max->getY()), tree->vertexId());

    neighbors[QUAD_L].front()->v[2] = v[7];
    neighbors[QUAD_L].back()->v[4] = v[7];

    tree->add(v[7]);
  }
}

void QuadtreeCell::makeOuterEdges() {
  if (subdivided()) {
    for (int i = 0; i < 4; i++) {
      children[i]->makeOuterEdges();
    }

    return;
  }

  if (inBoundary()) {
    return;
  }

  // cout << "criando arestas" << endl;

  if (!e[0]) {
    e[0] = makeE(0);

    if (neighbors[QUAD_B].front()->inBoundary()) {
      tree->addFront(e[0]);
    } else {
      if ((neighbors[QUAD_B].size() == 1) &&
          (neighbors[QUAD_B].front()->level < level) &&
          (neighbors[QUAD_B].front()->neighbors[QUAD_T].back() == this)) {
        neighbors[QUAD_B].front()->e[5] = e[0];
      } else {
        neighbors[QUAD_B].front()->e[4] = e[0];
      }
    }

    tree->add(e[0]);

    if (this->level < neighbors[QUAD_B].front()->level) {
      e[0]->setCell(this);
    } else {
      e[0]->setCell(neighbors[QUAD_B].front());
    }
  }

  // cout << "aresta 0" << endl;

  if ((!e[1]) && (neighbors[QUAD_B].size() == 2)) {
    e[1] = makeE(1);

    if (neighbors[QUAD_B].back()->inBoundary()) {
      tree->addFront(e[1]);
    } else {
      neighbors[QUAD_B].back()->e[4] = e[1];
    }

    tree->add(e[1]);

    e[1]->setCell(neighbors[QUAD_B].back());
  }

  ////cout << "aresta 1" << endl;

  if (!e[2]) {
    e[2] = makeE(2);

    if (neighbors[QUAD_R].front()->inBoundary()) {
      tree->addFront(e[2]);
    } else {
      if ((neighbors[QUAD_R].size() == 1) &&
          (neighbors[QUAD_R].front()->level < level) &&
          (neighbors[QUAD_R].front()->neighbors[QUAD_L].back() == this)) {
        neighbors[QUAD_R].front()->e[7] = e[2];
      } else {
        neighbors[QUAD_R].front()->e[6] = e[2];
      }
    }

    tree->add(e[2]);

    if (this->level < neighbors[QUAD_R].front()->level) {
      e[2]->setCell(this);
    } else {
      e[2]->setCell(neighbors[QUAD_R].front());
    }
  }

  ////cout << "aresta 2" << endl;

  if ((!e[3]) && (neighbors[QUAD_R].size() == 2)) {
    e[3] = makeE(3);

    if (neighbors[QUAD_R].back()->inBoundary()) {
      tree->addFront(e[3]);
    } else {
      neighbors[QUAD_R].back()->e[6] = e[3];
    }

    tree->add(e[3]);

    e[3]->setCell(neighbors[QUAD_R].back());
  }

  ////cout << "aresta 3" << endl;

  if (!e[4]) {
    e[4] = makeE(4);

    if (neighbors[QUAD_T].front()->inBoundary()) {
      tree->addFront(e[4]);
    } else {
      if ((neighbors[QUAD_T].size() == 1) &&
          (neighbors[QUAD_T].front()->level < level) &&
          (neighbors[QUAD_T].front()->neighbors[QUAD_B].back() == this)) {
        neighbors[QUAD_T].front()->e[1] = e[4];
      } else {
        neighbors[QUAD_T].front()->e[0] = e[4];
      }
    }

    tree->add(e[4]);

    if (this->level < neighbors[QUAD_T].front()->level) {
      e[4]->setCell(this);
    } else {
      e[4]->setCell(neighbors[QUAD_T].front());
    }
  }

  // cout << "aresta 4" << endl;

  if ((!e[5]) && (neighbors[QUAD_T].size() == 2)) {
    e[5] = makeE(5);

    if (neighbors[QUAD_T].back()->inBoundary()) {
      tree->addFront(e[5]);
    } else {
      neighbors[QUAD_T].back()->e[0] = e[5];
    }

    tree->add(e[5]);

    e[5]->setCell(neighbors[QUAD_T].back());
  }

  ////cout << "aresta 5" << endl;

  if (!e[6]) {
    e[6] = makeE(6);

    if (neighbors[QUAD_L].front()->inBoundary()) {
      tree->addFront(e[6]);
    } else {
      if ((neighbors[QUAD_L].size() == 1) &&
          (neighbors[QUAD_L].front()->level < level) &&
          (neighbors[QUAD_L].front()->neighbors[QUAD_R].back() == this)) {
        neighbors[QUAD_L].front()->e[3] = e[6];
      } else {
        neighbors[QUAD_L].front()->e[2] = e[6];
      }
    }

    tree->add(e[6]);

    if (this->level < neighbors[QUAD_L].front()->level) {
      e[6]->setCell(this);
    } else {
      e[6]->setCell(neighbors[QUAD_L].front());
    }
  }

  // cout << "aresta 6" << endl;

  if ((!e[7]) && (neighbors[QUAD_L].size() == 2)) {
    e[7] = makeE(7);

    if (neighbors[QUAD_L].back()->inBoundary()) {
      tree->addFront(e[7]);
    } else {
      neighbors[QUAD_L].back()->e[2] = e[7];
    }

    tree->add(e[7]);

    e[7]->setCell(neighbors[QUAD_L].back());
  }

  ////cout << "aresta 7" << endl;
}

void QuadtreeCell::makeTemplateBasedMesh() {
  if (subdivided()) {
    for (int i = 0; i < 4; i++) {
      children[i]->makeTemplateBasedMesh();
    }

    return;
  }

  if (inBoundary()) {
    return;
  }

  // gera a malha interna baseada em templates.

  // algumas restricoes:
  // os vertices utilizados por celulas vizinhas tem que ser os mesmos;
  // as arestas utilizadas por celulas vizinhas tem que ser as mesmas;
  // cada vertice, aresta, ou face criada tem que ser inserido na arvore
  // quando uma celula tem um vizinho que faz parte da borda, as arestas
  //	adjacente aos dois vizinhos tem que ser inseridas na fronteira da
  //	arvore
  // as faces criadas tem que estar de acordo com a regra da mao direita
  // as arestas criadas devem ter o lado esquerdo para fora da celula

  // algumas observacoes:
  // se os metodos sortNeighbors(), makeOuterVertices() e makeOuterEdges()
  // tiverem sido chamados, basta criar os vertices, arestas e triangulos
  // internos a celula, pois os vertices e as arestas adjacentes a duas
  // celulas jah foram criadas, da seguinte forma:
  //
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      e[6]                                e[3]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      v[7]                                v[3]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      e[7]                                e[2]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //	v[0] --- e[0] --- v[1] --- e[1] --- v[2]
  //
  // onde v[] sao os vertices, e e[] sao as arestas.
  //
  // o caso acima mostrado eh quando todas as celulas adjacentes sao
  // mais subdividas que a mostrada. no caso em que uma celula adjacente
  // tem o mesmo nivel, ou um nivel menor, temos o seguinte padrao:
  //
  //	v[6] --- e[5] --- v[5] --- e[4] --- v[4]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      e[6]                                  |
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      v[7]                                e[2]
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //      e[7]                                  |
  //        |                                   |
  //        |                                   |
  //        |                                   |
  //	v[0] --- e[0] --- v[1] --- e[1] --- v[2]
  //
  // nesse caso, a celula adjacente a direita eh de mesmo nivel ou de
  // nivel menor. entao, v[1] = NULL e e[1] = NULL. o mesmo acontece
  // para os outros casos, das celulas adjacentes abaixo, acima e a
  // direita

  int numNeighs[4], total = 0;

  for (int i = 0; i < 4; i++) {
    numNeighs[i] = neighbors[i].size();

    total += numNeighs[i];
  }

  if (total == 4) {
    // nenhuma das celulas adjacentes eh menor que essa

    ////cout << "tipo 1" << endl;

    makeMeshType1();
  } else if (total == 5) {
    // uma das celulas adjacentes eh menor que essa;

    ////cout << "tipo 2" << endl;

    for (int i = 0; i < 4; i++) {
      if (numNeighs[i] == 2) {
        makeMeshType2(i);

        break;
      }
    }
  } else if (total == 6) {
    // duas das celulas adjacentes sao vizinhas

    if (numNeighs[QUAD_B] == numNeighs[QUAD_T]) {
      ////cout << "tipo 3" << endl;

      if (numNeighs[QUAD_B] == 2) {
        makeMeshType3(QUAD_B);
      } else {
        makeMeshType3(QUAD_R);
      }
    } else {
      ////cout << "tipo 4" << endl;

      for (int i = 0; i < 4; i++) {
        if ((numNeighs[i] == numNeighs[(i + 1) % 4]) && (numNeighs[i] == 2)) {
          makeMeshType4(i);

          break;
        }
      }
    }
  } else if (total == 7) {
    ////cout << "tipo 5" << endl;

    for (int i = 0; i < 4; i++) {
      if (numNeighs[i] == 1) {
        makeMeshType5(i);

        break;
      }
    }
  } else {
    ////cout << "tipo 6" << endl;

    makeMeshType6();
  }

  ////cout << "fim criar triangulos template" << endl;
}

string QuadtreeCell::getText() {
  ostringstream stream;

  stream << id << " level " << level << " minLevel";

  string s = stream.str();

  return s;
}
