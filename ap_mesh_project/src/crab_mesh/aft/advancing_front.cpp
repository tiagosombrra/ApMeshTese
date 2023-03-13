#include "../../../include/crab_mesh/aft/advancing_front.h"

#include <memory>

AdvancingFront::AdvancingFront(double factor, [[maybe_unused]] double tolerance,
                               unsigned int numImproves, double phi)
    : Shape() {
  boundary = std::make_shared<Boundary>();
  quadtree = std::make_shared<Quadtree>(boundary, factor);

  // cout << "AdvancingFront tolerance: " << tolerance << endl;
  // Shape::setTolerance(tolerance);

  boundarySorted = true;
  this->numImproves = numImproves;
  this->phi = phi;

  lastVertexId = lastEdgeId = lastFaceId = 0;
}

AdvancingFront::AdvancingFront(std::shared_ptr<Boundary> boundary,
                               std::shared_ptr<Quadtree> quadtree,
                               [[maybe_unused]] double tolerance,
                               unsigned int numImproves)
    : Shape() {
  setBoundary(boundary);
  setQuadtree(quadtree);

  // cout << "AdvancingFront tolerance: " << tolerance << endl;
  // Shape::setTolerance(tolerance);

  setBoundarySorted(true);
  setNumImproves(numImproves);

  lastVertexId = lastEdgeId = lastFaceId = 0;
  phi = 0.5;
}

void AdvancingFront::makeInitialFront() {
  front = boundary->getEdges();

  for (EdgeList::iterator iter = front.begin(); iter != front.end(); iter++) {
    /*//cout << "aresta (" <<
        (*iter)->getV1()->getX() << ", " << (*iter)->getV1()->getY() << ") e ("
       <<
        (*iter)->getV2()->getX() << ", " << (*iter)->getV2()->getY() << ")" <<
       endl;*/

    vertices.push_back((*iter)->getV1());

    if ((*iter)->getV1()->getId() > lastVertexId) {
      lastVertexId = (*iter)->getV1()->getId();
    }

    edges.push_back((*iter));

    if ((*iter)->getId() > lastEdgeId) {
      lastEdgeId = (*iter)->getId();
    }
  }

  EdgeList quadtreeFront = quadtree->getFront();

  while (!quadtreeFront.empty()) {
    std::shared_ptr<Edge> e = quadtreeFront.front();
    quadtreeFront.pop_front();

    front.push_back(e);

    bool found = false;

    for (VertexList::iterator iter = vertices.begin(); iter != vertices.end();
         iter++) {
      if ((*iter) == e->getV1()) {
        found = true;

        break;
      }
    }

    if (!found) {
      vertices.push_back(e->getV1());

      if (e->getV1()->getId() > lastVertexId) {
        lastVertexId = e->getV1()->getId();
      }
    }

    edges.push_back(e);

    if (e->getId() > lastEdgeId) {
      lastEdgeId = e->getId();
    }
  }

  lastVertexId = quadtree->vertexId() - 1;
  lastEdgeId = quadtree->edgeId() - 1;
  lastFaceId = quadtree->faceId() - 1;
}

void AdvancingFront::sortFront() {
  EdgeList sorted, inBoundary;

  while (!front.empty()) {
    std::shared_ptr<Edge> least = front.front();

    if (least->isInBoundary()) {
      front.pop_front();

      inBoundary.push_back(least);

      continue;
    }

    EdgeList::iterator leastIter = front.begin();

    for (EdgeList::iterator iter = ++front.begin(); iter != front.end();
         iter++) {
      if ((*iter)->length() < least->length()) {
        least = (*iter);
        leastIter = iter;
      }
    }

    front.erase(leastIter);

    sorted.push_back(least);
  }

  front.swap(sorted);

  while (!inBoundary.empty()) {
    std::shared_ptr<Edge> least = inBoundary.front();
    EdgeList::iterator leastIter = inBoundary.begin();

    for (EdgeList::iterator iter = ++inBoundary.begin();
         iter != inBoundary.end(); iter++) {
      if ((*iter)->length() < least->length()) {
        least = (*iter);
        leastIter = iter;
      }
    }

    inBoundary.erase(leastIter);

    sorted.push_back(least);
  }

  front.insert(front.begin(), sorted.begin(), sorted.end());
}

bool AdvancingFront::interceptionTest(std::shared_ptr<Edge> e,
                                      std::shared_ptr<Vertex> candidate,
                                      bool inFaceTest, bool onlyFrontEdges) {
  bool intercepts = false;

  EdgeList *testEdges = &edges;

  if (onlyFrontEdges) {
    testEdges = &front;
  }

  for (EdgeList::iterator iter = testEdges->begin(); iter != testEdges->end();
       iter++) {
    if (((*iter)->intercept(e->getV1(), candidate)) ||
        ((*iter)->intercept(candidate, e->getV2())) ||
        false)  //((*iter)->intercept(candidate)))
    {
      intercepts = true;

      break;
    }
  }

  if (inFaceTest && !intercepts) {
    // static Face f;
    Face f;
    // cout << "inte teste" << endl;

    f.setVertices(e->getV1(), e->getV2(), candidate);

    for (VertexList::iterator iter = frontVertices.begin();
         iter != frontVertices.end(); iter++) {
      // #pragma omp critical
      //             {

      //            if (!(*iter)) {
      //               cout<<"iter: "<<(*iter)->getId()<<" thread:
      //               "<<omp_get_thread_num()<<endl;
      //            }
      //            }

      if (((*iter) == e->getV1()) || ((*iter) == e->getV2()) ||
          ((*iter) == candidate)) {
        continue;
      }

      if (((*iter)->matches(e->getV1())) || ((*iter)->matches(e->getV2())) ||
          ((*iter)->matches(candidate))) {
        continue;
      }

      // if (f->in((*iter)))
      if (!f.out((*iter))) {
        intercepts = true;

        break;
      }
    }

    f.setVertices(nullptr, nullptr, nullptr);

    // cout << "fim inte teste" << endl;
  }

  return intercepts;
}

bool AdvancingFront::interceptionTest(std::shared_ptr<Edge> e) {
  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter) == e) {
      continue;
    }

    if ((*iter)->equals(e)) {
      continue;
    }

    if ((*iter)->intercept(e->getV1(), e->getV2())) {
      return true;
    }
  }

  return false;
}

std::shared_ptr<Vertex> AdvancingFront::makeIdealVertex(std::shared_ptr<Edge> e,
                                                        double &h) {
  std::shared_ptr<Vertex> v = e->normal();

  h *= e->getCell()->height();

  v->scalarMultiplication(h);
  v->sum(e->getMid());

  // #if DEBUG_MODE
  //     v->h = h;
  // #endif //#if DEBUG_MODE

  return v;
}

bool AdvancingFront::findBestVertex(std::shared_ptr<Edge> e,
                                    std::shared_ptr<Vertex> &best,
                                    bool geometryPhase) {
  bool vertexExistedBefore = true;

  double h = 1.0;

  best = (geometryPhase) ? makeIdealVertex(e, h) : nullptr;

  VertexList bestVertices;

  // debug markos
  // std::cerr << std::boolalpha << "geometryPhase = " << geometryPhase << ";
  // frontVertices.size() = " << frontVertices.size() << "; "; int matches = 0,
  // outbox = 0, outdistance = 0, outnormal = 0, intercepted = 0; endebug markos

  for (VertexList::iterator iter = frontVertices.begin();
       iter != frontVertices.end(); iter++) {
    std::shared_ptr<Vertex> candidate = (*iter);

    if ((candidate == e->getV1()) || (candidate == e->getV2()) ||
        (candidate->matches(e->getV1())) || (candidate->matches(e->getV2()))) {
      // matches++;
      continue;
    }

    if (geometryPhase) {
      if ((candidate->getX() < best->getX() - h) ||
          (candidate->getX() > best->getX() + h) ||
          (candidate->getY() < best->getY() - h) ||
          (candidate->getY() > best->getY() + h)) {
        // outbox++;
        continue;
      }

      if (candidate->distance(best) > h) {
        // outdistance++;
        continue;
      }

      // testa se best estah perto demais de e
      double dist = e->straightDistance(candidate);

      if (dist < 0.0) {
        // cout << "debug " << e->getId() << endl;
        best = nullptr;

        // std::cerr << "matches = " << matches << "; outbox = " << outbox << ";
        // outdistance = " << outdistance << "; outnormal = " << outnormal <<
        //            "; intercepted = " << intercepted << ";
        //            bestVertices.size() = " << bestVertices.size() << ";
        //            return = false, nullptr; reason = dist < 0.0" <<
        //            std::endl;

        return false;
      } else if (dist <= h * 0.1) {
        // continue;

        // std::cerr << "matches = " << matches << "; outbox = " << outbox << ";
        // outdistance = " << outdistance << "; outnormal = " << outnormal <<
        //            "; intercepted = " << intercepted << ";
        //            bestVertices.size() = " << bestVertices.size() << ";
        //            return = false,  nullptr; reason = dist < h*0.1" <<
        //            std::endl;

        bestVertices.clear();

        best = nullptr;

        return false;
      }
    } else {
      if (!(e->accordingToNormal(candidate))) {
        // outnormal++;
        continue;
      }
    }

    // bool intercept = interceptionTest(e, candidate, false);
    // bool intercept = geometryPhase ? interceptionTest(e, candidate, false) :
    // interceptionTest(e, candidate, false, true);
    bool intercept = interceptionTest(e, candidate, true, false);

    if (!intercept) {
      bestVertices.push_back(candidate);
    }
    // else
    //{
    // intercepted++;
    //}
  }

  // std::cerr << "matches = " << matches << "; outbox = " << outbox << ";
  // outdistance = " << outdistance << "; outnormal = " << outnormal <<
  //            "; intercepted = " << intercepted << "; bestVertices.size() = "
  //            << bestVertices.size() << "; return = ";

  if (geometryPhase && bestVertices.empty()) {
    bool intercept = interceptionTest(e, best);

    if (intercept) {
      best = nullptr;

      // std::cerr << "false,  nullptr; reason = intercept == true" <<
      // std::endl;

      return false;
    } else {
      bool tooClose = false;

      // testa se best estah perto demais de alguma outra edge da fronteira

      for (EdgeList::iterator iter = edges.begin(); iter != edges.end();
           iter++) {
        if ((*iter)->intercept(best)) {
          tooClose = true;

          break;
        }
      }

      if (tooClose) {
        best = nullptr;

        // std::cerr << "false,  nullptr; reason = tooClose == true" <<
        // std::endl;

        return false;
      } else {
        best->setId(++lastVertexId);

        frontVertices.push_back(best);
        innerVertices.push_back(best);
        vertices.push_back(best);
      }
    }

    // std::cerr << "false, NOTNULL; reason = std::make_shared<vertex not close
    // to existing"
    // << std::endl;

    vertexExistedBefore = false;
  } else if (!bestVertices.empty()) {
    if (geometryPhase) {
      best = nullptr;
    }

    best = bestVertices.front();
    bestVertices.pop_front();

    std::shared_ptr<Vertex> v1 = std::make_shared<Vertex>(
        e->getV1()->getX() - best->getX(), e->getV1()->getY() - best->getY());
    std::shared_ptr<Vertex> v2 = std::make_shared<Vertex>(
        e->getV2()->getX() - best->getX(), e->getV2()->getY() - best->getY());

    double maxAngle = v1->angle(v2);

    while (!bestVertices.empty()) {
      std::shared_ptr<Vertex> candidate = bestVertices.front();
      bestVertices.pop_front();

      v1->setPosition(e->getV1()->getX() - candidate->getX(),
                      e->getV1()->getY() - candidate->getY());
      v2->setPosition(e->getV2()->getX() - candidate->getX(),
                      e->getV2()->getY() - candidate->getY());

      double angle = v1->angle(v2);

      if (angle > maxAngle) {
        maxAngle = angle;
        best = candidate;
      } else if (fabs(maxAngle - angle) < TOLERANCE_AFT) {
        // caso em que os 2 vertices, candidate e best sao geometricamente
        // iguais, mas topologicamente diferentes
        std::shared_ptr<Edge> candidateEdge1, candidateEdge2;
        candidateEdge1 = candidateEdge2 = nullptr;

        for (EdgeList::iterator iter = front.begin(); iter != front.end();
             iter++) {
          std::shared_ptr<Edge> eAux = (*iter);

          if (eAux->getV2() == candidate) {
            candidateEdge1 = eAux;
          }

          if (eAux->getV1() == candidate) {
            candidateEdge2 = eAux;
          }

          if (candidateEdge1 && candidateEdge2) {
            break;
          }
        }

        if ((candidateEdge1) && (candidateEdge2) &&
            (candidateEdge1->accordingToNormal(e->getV1(), true)) &&
            (candidateEdge1->accordingToNormal(e->getV2(), true)) &&
            (candidateEdge2->accordingToNormal(e->getV1(), true)) &&
            (candidateEdge2->accordingToNormal(e->getV2(), true))) {
          maxAngle = angle;
          best = candidate;
        }
      }
    }

    v1 = nullptr;
    v2 = nullptr;

    // std::cerr << "true, NOTNULL; reason = existing vertex" << std::endl;
  } else {
    // std::cerr << "false, NOTNULL; reason = std::make_shared<vertex" <<
    // std::endl;

    vertexExistedBefore = false;
  }

  return vertexExistedBefore;
}

void AdvancingFront::insertInFront(std::shared_ptr<Edge> last,
                                   std::shared_ptr<Edge> e) {
  if (boundarySorted) {
    if (last) {
      bool inserted = false;

      for (EdgeList::iterator iter = front.begin(); iter != front.end();
           iter++) {
        if ((*iter) == last) {
          for (iter++; iter != front.end(); iter++) {
            if ((*iter)->length() > e->length()) {
              front.insert(iter, e);

              inserted = true;

              break;
            }
          }

          break;
        }
      }

      if (!inserted) {
        front.push_back(e);
      }
    } else {
      bool inserted = false;

      for (EdgeList::iterator iter = front.begin(); iter != front.end();
           iter++) {
        if ((*iter)->length() > e->length()) {
          front.insert(iter, e);

          inserted = true;

          break;
        }
      }

      if (!inserted) {
        front.push_back(e);
      }
    }
  } else {
    front.push_back(e);
  }
}

std::shared_ptr<Edge> AdvancingFront::findEdge(std::shared_ptr<Vertex> v1,
                                               std::shared_ptr<Vertex> v2) {
  std::shared_ptr<Edge> e = nullptr;

  for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
    if ((*iter)->equals(v1, v2)) {
      e = (*iter);
      break;
    }
  }

  return e;
}

EdgeList AdvancingFront::findAdjacentEdges(std::shared_ptr<Vertex> v) {
  EdgeList adjacency;

  EdgeSet adjacentEdges = v->getAdjacentEdges();

  adjacency.insert(adjacency.end(), adjacentEdges.begin(), adjacentEdges.end());

  return adjacency;
}

FaceList AdvancingFront::findAdjacentFaces(const FaceList &faces,
                                           std::shared_ptr<Vertex> v) {
  FaceList adjacency;

  for (FaceList::const_iterator iter = faces.begin(); iter != faces.end();
       iter++) {
    if (((*iter)->getV1() == v) || ((*iter)->getV2() == v) ||
        ((*iter)->getV3() == v)) {
      adjacency.push_back((*iter));
    }
  }

  return adjacency;
}

void AdvancingFront::removeFromFront(std::shared_ptr<Vertex> v1,
                                     std::shared_ptr<Vertex> v2,
                                     std::shared_ptr<Vertex> v3) {
  // verifica se v1 eh adjacente a alguma aresta livre, ou seja,
  // da fronteira corrente. se for adjacente, nao remove. se nao for
  // adjacente, remove
  EdgeSet adjacentEdges = v1->getAdjacentEdges();

  bool remove = true;

  for (EdgeSet::iterator iter = adjacentEdges.begin();
       iter != adjacentEdges.end(); iter++) {
    if ((*iter)->isFree()) {
      remove = false;

      break;
    }
  }

  if (remove) {
    frontVertices.remove(v1);
  }

  // verifica se v2 eh adjacente a alguma aresta livre, ou seja,
  // da fronteira corrente. se for adjacente, nao remove. se nao for
  // adjacente, remove
  adjacentEdges = v2->getAdjacentEdges();

  remove = true;

  for (EdgeSet::iterator iter = adjacentEdges.begin();
       iter != adjacentEdges.end(); iter++) {
    if ((*iter)->isFree()) {
      remove = false;

      break;
    }
  }

  if (remove) {
    frontVertices.remove(v2);
  }

  // verifica se v3 eh adjacente a alguma aresta livre, ou seja,
  // da fronteira corrente. se for adjacente, nao remove. se nao for
  // adjacente, remove
  adjacentEdges = v3->getAdjacentEdges();

  remove = true;

  for (EdgeSet::iterator iter = adjacentEdges.begin();
       iter != adjacentEdges.end(); iter++) {
    if ((*iter)->isFree()) {
      remove = false;

      break;
    }
  }

  if (remove) {
    frontVertices.remove(v3);
  }
}

enum MethodStatus AdvancingFront::makeMesh(bool frontBased,
                                           bool geometryPhase) {
  // cout << "AdvancingFront::makeMesh(frontBased = " << frontBased << ",
  // geometryPhase = " << geometryPhase << ")" << endl;

  frontVertices.clear();

  for (EdgeList::iterator iter = front.begin(); iter != front.end(); iter++) {
    frontVertices.push_back((*iter)->getV1());
  }

  std::shared_ptr<Edge> last = nullptr;

  if (frontBased && geometryPhase) {
    last = std::make_shared<Edge>();
    front.push_back(last);
  }

  while (true) {
    std::shared_ptr<Edge> e = nullptr;

    if (frontBased) {
      if (front.empty()) {
        break;
      }

      e = front.front();
      front.pop_front();
    } else {
      if (rejected.empty()) {
        break;
      }

      e = rejected.front();
      rejected.pop_front();
    }

    if (frontBased && (e == last)) {
      last = nullptr;
      continue;
    }

    std::shared_ptr<Vertex> best = nullptr;
    bool existed = findBestVertex(e, best, geometryPhase);

    if (!best) {
      if (frontBased) {
        rejected.push_back(e);

        continue;
      } else {
        // #if USE_OPENGL
        //                 e->setColor(1.0, 0.0, 0.0);

        //                for (EdgeList::iterator iter = rejected.begin();
        //                     iter != rejected.end(); iter++)
        //                {
        //                    (*iter)->setColor(1.0, 0.0, 1.0);
        //                }
        // #endif //#if USE_OPENGL
        rejected.push_front(e);

        return (geometryPhase) ? ADVF_GEOMETRY_EDGE_REJECTED_TWICE
                               : ADVF_TOPOLOGY_EDGE_REJECTED_TWICE;
      }
    }

    if (geometryPhase && !existed && !quadtree->in(best)) {
      if (frontBased) {
        rejected.push_back(e);
      } else {
        front.push_back(e);
      }

      vertices.remove(best);
      innerVertices.remove(best);
      frontVertices.remove(best);

      best = nullptr;

      continue;
    }

    std::shared_ptr<Edge> e1, e2;
    e1 = e2 = nullptr;

    if (existed) {
      e1 = findEdge(e->getV1(), best);
      e2 = findEdge(best, e->getV2());
    }

    if (e1) {
      if (frontBased) {
        front.remove(e1);
      }

      rejected.remove(e1);

      e1->setFree(false);
    } else {
      e1 = std::make_shared<Edge>(e->getV1(), best, ++lastEdgeId);

      e->getV1()->addAdjacentEdge(e1);
      best->addAdjacentEdge(e1);

      innerEdges.push_back(e1);
      edges.push_back(e1);

      if (frontBased) {
        insertInFront(last, e1);
      } else {
        rejected.push_back(e1);
      }

      quadtree->findCell(e1);
    }

    if (e2) {
      if (frontBased) {
        front.remove(e2);
      }

      rejected.remove(e2);

      e2->setFree(false);
    } else {
      e2 = std::make_shared<Edge>(best, e->getV2(), ++lastEdgeId);

      e->getV2()->addAdjacentEdge(e2);
      best->addAdjacentEdge(e2);

      innerEdges.push_back(e2);
      edges.push_back(e2);

      if (frontBased) {
        insertInFront(last, e2);
      } else {
        rejected.push_back(e2);
      }

      quadtree->findCell(e2);
    }

    e->setFree(false);

    removeFromFront(e->getV1(), e->getV2(), best);

    std::shared_ptr<Face> f =
        std::make_shared<Face>(e->getV1(), e->getV2(), best, ++lastFaceId);

    mesh.push_back(f);
  }

  return geometryPhase ? ADVF_GEOMETRY_MESH_DONE : ADVF_TOPOLOGY_MESH_DONE;
}

void AdvancingFront::fillMesh() {
  VertexList quadVertices = quadtree->getVertices();

  while (!quadVertices.empty()) {
    std::shared_ptr<Vertex> v = quadVertices.front();
    quadVertices.pop_front();

    innerVertices.push_back(v);

    bool found = false;

    for (VertexList::iterator iter = vertices.begin(); iter != vertices.end();
         iter++) {
      if ((*iter) == v) {
        found = true;

        break;
      }
    }

    if (!found) {
      vertices.push_back(v);
    }
  }

  EdgeList quadEdges = quadtree->getEdges();

  while (!quadEdges.empty()) {
    std::shared_ptr<Edge> e = quadEdges.front();
    quadEdges.pop_front();

    innerEdges.push_back(e);

    bool found = false;

    for (EdgeList::iterator iter = edges.begin(); iter != edges.end(); iter++) {
      if ((*iter) == e) {
        found = true;

        break;
      }
    }

    if (!found) {
      edges.push_back(e);
    }
  }

  FaceList quadMesh = quadtree->getMesh();

  mesh.insert(mesh.end(), quadMesh.begin(), quadMesh.end());
}

bool AdvancingFront::laplacianSmoothing(bool &changed) {
  changed = false;

  static const bool testNegativeFaces = false;

  for (VertexList::iterator iter = innerVertices.begin();
       iter != innerVertices.end(); iter++) {
    std::shared_ptr<Vertex> v = (*iter);

    double weight = 1.0;

    double oldx = v->getX();
    double oldy = v->getY();

    double numx, numy, den;
    numx = numy = den = 0.0;

    EdgeList adjacency = findAdjacentEdges(v);

    while (!adjacency.empty()) {
      std::shared_ptr<Edge> e = adjacency.front();

      adjacency.pop_front();

      std::shared_ptr<Vertex> v2 = (e->getV1() == v) ? e->getV2() : e->getV1();

      numx += weight * (v2->getX() - oldx);
      numy += weight * (v2->getY() - oldy);

      den += weight;
    }

    den = (den > TOLERANCE_AFT) ? phi / den : 0;

    numx *= den;
    numy *= den;

    double newx = oldx + numx;
    double newy = oldy + numy;

    v->setPosition(newx, newy);

    if (testNegativeFaces) {
      FaceList faces = findAdjacentFaces(mesh, v);

      bool negativeSurface = false;

      for (FaceList::iterator iter2 = faces.begin(); iter2 != faces.end();
           iter2++) {
        if ((*iter2)->orientedSurface() <= TOLERANCE_AFT) {
          negativeSurface = true;

          break;
        }
      }

      if (negativeSurface) {
        v->setPosition(oldx, oldy);
      } else {
        changed = true;
      }
    } else {
      changed = true;
    }
  }

  // para ajeitar o mid e o vector das innerEdges
  for (EdgeList::iterator iter = innerEdges.begin(); iter != innerEdges.end();
       iter++) {
    (*iter)->setVertices((*iter)->getV1(), (*iter)->getV2());
  }

  return true;
}

void AdvancingFront::setBoundarySorted(bool boundarySorted) {
  this->boundarySorted = boundarySorted;
}

void AdvancingFront::setNumImproves(unsigned int numImproves) {
  this->numImproves = numImproves;
}

unsigned int AdvancingFront::getNumImproves() { return numImproves; }

bool AdvancingFront::isBoundarySorted() { return boundarySorted; }

// double AdvancingFront::getTolerance()
//{
//    return Shape::tolerance;
//}

void AdvancingFront::setBoundary(std::shared_ptr<Boundary> boundary) {
  this->boundary = boundary;
}

std::shared_ptr<Boundary> AdvancingFront::getBoundary() { return boundary; }

void AdvancingFront::setQuadtree(std::shared_ptr<Quadtree> quadtree) {
  this->quadtree = quadtree;
}

std::shared_ptr<Quadtree> AdvancingFront::getQuadtree() { return quadtree; }

VertexList AdvancingFront::getVertices() { return vertices; }

VertexList AdvancingFront::getInnerVertices() { return innerVertices; }

EdgeList AdvancingFront::getEdges() { return edges; }

EdgeList AdvancingFront::getInnerEdges() { return innerEdges; }

FaceList AdvancingFront::getMesh() { return mesh; }

void AdvancingFront::addVertices(VertexList vertices) {
  if (!lastVertexId) {
    lastVertexId = boundary->getLastVertexId();
  }

  while (!vertices.empty()) {
    std::shared_ptr<Vertex> v = vertices.front();
    vertices.pop_front();

    v->setId(++lastVertexId);

    this->vertices.push_back(v);
    this->innerVertices.push_back(v);
    this->frontVertices.push_back(v);
  }
}

void AdvancingFront::addEdges(EdgeList edges) {
  if (!lastEdgeId) {
    lastEdgeId = boundary->getLastEdgeId();
  }

  while (!edges.empty()) {
    std::shared_ptr<Edge> e = edges.front();
    edges.pop_front();

    e->setId(++lastEdgeId);

    this->edges.push_back(e);
    this->innerEdges.push_back(e);
  }
}

void AdvancingFront::addMesh(FaceList mesh) {
  while (!mesh.empty()) {
    std::shared_ptr<Face> f = mesh.front();
    mesh.pop_front();

    f->setId(++lastFaceId);

    this->mesh.push_back(f);
  }
}

bool AdvancingFront::belongsToAdvFront(std::shared_ptr<Edge> e) {
  for (EdgeList::iterator iter = front.begin(); iter != front.end(); iter++) {
    if ((*iter) == e) {
      return true;
    }
  }

  return false;
}

enum MethodStatus AdvancingFront::makeGeometryBasedMesh() {
  if (front.size() < 3) {
    return ADVF_EMPTY;
  }

  if (front.size() == 3) {
    std::shared_ptr<Vertex> v = (*(++front.begin()))->getV1();
    std::shared_ptr<Face> f = std::make_shared<Face>(
        front.front()->getV1(), v, front.back()->getV1(), ++lastFaceId);
    mesh.push_back(f);

    return ADVF_GEOMETRY_MESH_DONE;
  }

  if (boundarySorted) {
    sortFront();
  }

  enum MethodStatus status = makeMesh(true);

  if (status == ADVF_GEOMETRY_MESH_DONE) {
    status = makeMesh(false);
  }

  return status;
}

enum MethodStatus AdvancingFront::makeTopologyBasedMesh() {
  // cout << "AdvancingFront::makeTopologyBasedMesh()" << endl;

  if (rejected.empty()) {
    return ADVF_TOPOLOGY_MESH_DONE;
  }

  while (!rejected.empty()) {
    std::shared_ptr<Edge> e = rejected.front();
    rejected.pop_front();

    // #if USE_OPENGL
    //         if (boundary->belongs(e))
    //         {
    //             //figura
    //             //e->setColor(1.0, 1.0, 1.0);
    //             e->setColor(0.0, 0.0, 0.0);
    //             //endfigura
    //         }
    //         else
    //         {
    //             e->setColor(0.0, 0.0, 1.0);
    //         }
    // #endif //#if USE_OPENGL

    front.push_back(e);
  }

  enum MethodStatus status = makeMesh(true, false);

  if (!rejected.empty()) {
    // cout << "rejected nao tah vazio, mas era para estar" << endl;
  }

  if (status == ADVF_TOPOLOGY_MESH_DONE) {
    status = makeMesh(false, false);
  }

  return status;
}

enum MethodStatus AdvancingFront::improveMesh() {
  for (unsigned int i = 0; i < numImproves; i++) {
    bool movedSomeVertex;

    laplacianSmoothing(movedSomeVertex);
  }

  return IMPR_IMPROVEMENT_DONE;
}

bool AdvancingFront::execute(const FaceList &oldmesh) {
#if USE_PRINT_COMENT
  cout << "tolerancia usada pelo AFT " << Shape::tolerance << endl;
#endif  // #if USE_PRINT_COMENT

  enum MethodStatus status;

  // FaceList emptymesh;

  status = quadtree->generate(oldmesh);

  // cout << methodNotices[status] << endl;

  if (status != QUAD_INITIAL_TREE_DONE) {
    return false;
  }

#if USE_PRINT_COMENT
  cout << "gerou a quadtree" << endl;
#endif  // #if USE_PRINT_COMENT

  /*if (quadtree->refineToLevel() != QUAD_REFINE_TO_LEVEL_DONE)
  {
      return false;
  }*/

  status = quadtree->refineAccordingToNeighbors();

  // cout << methodNotices[status] << endl;

  if (status != QUAD_REFINE_ACCORDING_TO_NEIGHBORS_DONE) {
    return false;
  }

#if USE_PRINT_COMENT
  cout << "refinou a quadtree" << endl;
#endif  // #if USE_PRINT_COMENT

  if (USE_TEMPLATE == std::string("y")) {
    status = quadtree->makeTemplateBasedMesh();
    // cout << methodNotices[status] << endl;
    if (status != QUAD_MAKE_TEMPLATE_BASED_MESH_DONE) {
      return false;
    }
  }

#if USE_PRINT_COMENT
  cout << "gerou malha por templates" << endl;
#endif  // #if USE_PRINT_COMENT

  makeInitialFront();

  status = makeGeometryBasedMesh();

#if USE_PRINT_COMENT
  cout << "gerou malha por avanco de fronteira - geometria" << endl;
#endif  // #if USE_PRINT_COMENT

  if ((status != ADVF_GEOMETRY_MESH_DONE) &&
      (status != ADVF_GEOMETRY_EDGE_REJECTED_TWICE)) {
    return false;
  }

  if (status == ADVF_GEOMETRY_EDGE_REJECTED_TWICE) {
    status = makeTopologyBasedMesh();

    if (status != ADVF_TOPOLOGY_MESH_DONE) {
      return false;
    }
  }

#if USE_PRINT_COMENT
  cout << "gerou malha por avanco de fronteira - topologia" << endl;
#endif  // #if USE_PRINT_COMENT

  fillMesh();

  status = improveMesh();

  if (status != IMPR_IMPROVEMENT_DONE) {
    return false;
  }

#if USE_PRINT_COMENT
  cout << "melhorou a malha" << endl;
#endif  // #if USE_PRINT_COMENT

  return true;
}

string AdvancingFront::getText() {
  string s;

  return s;
}

// #if USE_OPENGL
//  void AdvancingFront::highlight()
//{

//}

// void AdvancingFront::unhighlight()
//{

//}

// void AdvancingFront::draw()
//{
//    static Edge e;

//    //figura
//    //e.setColor(0.0, 0.0, 1.0);
//    //endfigura

//    if (!edges.empty())
//    {
//        for (FaceList::iterator iter = mesh.begin();
//             iter != mesh.end(); iter++)
//        {
//            (*iter)->draw();
//        }

//        for (EdgeList::iterator iter = edges.begin();
//             iter != edges.end(); iter++)
//        {
//            (*iter)->draw();
//        }
//    }
//    else
//    {
//        for (FaceList::iterator iter = mesh.begin();
//             iter != mesh.end(); iter++)
//        {
//            e.setVertices((*iter)->getV1(), (*iter)->getV2());
//            e.draw();
//            e.setVertices((*iter)->getV2(), (*iter)->getV3());
//            e.draw();
//            e.setVertices((*iter)->getV3(), (*iter)->getV1());
//            e.draw();
//        }
//    }

//    for (VertexList::iterator iter = vertices.begin();
//         iter != vertices.end(); iter++)
//    {
//        (*iter)->highlight();

//        (*iter)->draw();

//        (*iter)->unhighlight();
//    }

//    e.setVertices(nullptr, nullptr);

// #if DEBUG_MODE
//     if (!vertices.empty())
//     {
//         vertices.back()->drawCircle();
//     }
// #endif //#if DEBUG_MODE
// }

// void AdvancingFront::drawNormals()
//{
//    for (EdgeList::iterator iter = edges.begin();
//         iter != edges.end(); iter++)
//    {
//        (*iter)->drawNormal();
//    }
//}
// #endif //#if USE_OPENGL
