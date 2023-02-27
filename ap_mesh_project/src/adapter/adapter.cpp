#include "../../include/adapter/adapter.h"

extern double TOLERANCE;
extern double TOLERANCE_AFT;
extern double TOLERANCE_CURVATURE;
extern double RATIO_AFT;
extern double SMOOTHING_LAPLACIAN_NUMBER;
extern double SMOOTHING_LAPLACIAN_FACTOR;
extern double DISCRETIZATION_CURVE_FACTOR;
extern double DISCRETIZATION_CURVE_FACTOR_INTERNAL;

#if USE_OPENMP
list<PointAdaptive *> Adapter::AdaptCurveByCurveOmp(
    CurveAdaptive *curve, Performer::IdManager *id_manager,
    double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<PointAdaptive *> points = curve->GetPoints();
  list<PointAdaptive *>::iterator point_current = points.begin();
  list<PointAdaptive *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  PointAdaptive midpoint;

  static_cast<CurveAdaptiveParametric *>(curve)->SortPointsByParameters();

  // 1.1. Inicialize a árvore binária com a raiz para toda a curva
  BinTree bin_tree;

  // 1.2. Para cada segmento da curva
  while (point_next != points.end()) {
    // o tamanho velho de cada segmento
    double length_old = 0;
    // o tamanho novo de cada segmento
    double lenght_new = 0;
    // tamanho paramétrico de cada segmento
    double lenght_par = 0;
    // parametro correspondente ao ponto central do segmento
    double midpoint_segment = 0;
    // pow ( 2, exp ); // o fator de rediscretização
    double factor_disc = DISCRETIZATION_CURVE_FACTOR;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old =
        curve->CalculateLengthPoints(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint =
        static_cast<CurveAdaptiveParametric *>(curve)
            ->CalculateMidpointByPoints(*(*point_current), *(*point_next));
    midpoint.SetId(id_manager->next(0));

    // 1.2.2.1 Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurveAdaptiveParametric *>(curve)->FindParameterByPoint(
            midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    ka_midpoint = curve->CalculateCurvature(midpoint_segment);

    kd_average =
        (curve->CalculateCurvature(0) + curve->CalculateCurvature(1)) / 2.0;

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->GetLength();

    bin_tree.Subdivide(midpoint_segment, lenght_par * factor_disc_global,
                       static_cast<CurveAdaptiveParametric *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.Restrict(static_cast<CurveAdaptiveParametric *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.Rediscretization();
  (static_cast<CurveAdaptiveParametric *>(curve))->UpdateParameters(parameters);

  list<PointAdaptive *> list_new_points;

  NodeAdaptive *point_front = static_cast<NodeAdaptive *>(points.front());
  point_front->SetId(id_manager->next(0));

  NodeAdaptive *point_back = static_cast<NodeAdaptive *>(points.back());
  point_back->SetId(id_manager->next(0));

  list_new_points.push_front(point_front);

  for (auto param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    NodeAdaptive *point_intermediate =
        new NodeAdaptive((static_cast<CurveAdaptiveParametric *>(curve))
                             ->FindPointByParameter((*param_iterator)));
    point_intermediate->SetId(id_manager->next(0));
    list_new_points.push_back(point_intermediate);
  }

  list_new_points.push_back(point_back);

  return list_new_points;

  //    for (list<double>::iterator it = ++parametros.begin();
  //         it != --parametros.end(); it++)
  //    {
  //        Noh *n = new Noh(((CurveAdaptiveParametric
  //        *)c)->Parameterize((*it))); n->id = idManager->next(0);
  //        c->inserePonto(n);
  //    }

  //    return c;
}

list<PointAdaptive *> Adapter::AdaptCurveBySurfaceOmp(
    CurveAdaptive *curve, Performer::IdManager *id_manager,
    double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<PointAdaptive *> points = curve->GetPoints();
  list<PointAdaptive *>::iterator point_current = points.begin();
  list<PointAdaptive *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  PointAdaptive midpoint;

  static_cast<CurveAdaptiveParametric *>(curve)->SortPointsByParameters();

  // 1.1. Inicialize a árvore binária com a raiz para toda a curva
  BinTree bin_tree;

  // 1.2. Para cada segmento da curva
  while (point_next != points.end()) {
    // o tamanho velho de cada segmento
    double length_old = 0;
    // o tamanho novo de cada segmento
    double lenght_new = 0;
    // tamanho paramétrico de cada segmento
    double lenght_par = 0;
    // parametro correspondente ao ponto central do segmento
    double midpoint_segment = 0;
    // pow ( 2, exp ); // o fator de rediscretização
    double factor_disc = DISCRETIZATION_CURVE_FACTOR;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old =
        curve->CalculateLengthPoints(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint =
        static_cast<CurveAdaptiveParametric *>(curve)
            ->CalculateMidpointByPoints(*(*point_current), *(*point_next));

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    CurvatureAnalytical ka_p0(midpoint,
                              *(static_cast<PatchCoons *>(curve->GetPatch(0))));
    ka_midpoint = ka_p0.CalculateGaussCurvature();

    if (curve->GetNumBerPatches() == 1) {
      // testamos se ka é ZERO!
      if (fabs(ka_midpoint) < TOLERANCE) {
        ka_midpoint = ka_p0.CalculateMeanCurvature();
        kd_average = ((static_cast<NodeAdaptive *>((*point_current)))->GetHd() +
                      (static_cast<NodeAdaptive *>((*point_next))->GetHd())) /
                     2.0;
      } else {
        kd_average = ((static_cast<NodeAdaptive *>((*point_current)))->GetGd() +
                      (static_cast<NodeAdaptive *>((*point_next)))->GetGd()) /
                     2.0;
      }
    } else {
      double Ha = ka_p0.CalculateMeanCurvature();

      for (unsigned int i = 1; i < curve->GetNumBerPatches(); i++) {
        CurvatureAnalytical ka_pi(
            midpoint, *(static_cast<PatchCoons *>(curve->GetPatch(i))));
        double Ga_pi = ka_pi.CalculateGaussCurvature();

        ka_midpoint = (fabs(ka_midpoint) > fabs(Ga_pi)) ? ka_midpoint : Ga_pi;

        // testamos se ka é ZERO!
        if (fabs(ka_midpoint) < TOLERANCE) {
          double Ha_pi = ka_pi.CalculateMeanCurvature();

          ka_midpoint = (Ha > Ha_pi) ? Ha : Ha_pi;
          kd_average =
              ((static_cast<NodeAdaptive *>((*point_current)))->GetHd() +
               (static_cast<NodeAdaptive *>((*point_next))->GetHd())) /
              2.0;
        } else {
          kd_average =
              ((static_cast<NodeAdaptive *>((*point_current)))->GetGd() +
               (static_cast<NodeAdaptive *>((*point_next)))->GetGd()) /
              2.0;
        }
      }
    }

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->GetLength();

    // 1.2.6. Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurveAdaptiveParametric *>(curve)->FindParameterByPoint(
            midpoint);

    bin_tree.Subdivide(midpoint_segment, lenght_par * factor_disc_global,
                       static_cast<CurveAdaptiveParametric *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.Restrict(static_cast<CurveAdaptiveParametric *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.Rediscretization();
  (static_cast<CurveAdaptiveParametric *>(curve))->UpdateParameters(parameters);

  list<PointAdaptive *> list_new_points;

  NodeAdaptive *point_front = static_cast<NodeAdaptive *>(points.front());
  point_front->SetId(id_manager->next(0));
  list_new_points.push_front(point_front);

  for (auto param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    NodeAdaptive *n =
        new NodeAdaptive((static_cast<CurveAdaptiveParametric *>(curve))
                             ->FindPointByParameter((*param_iterator)));
    n->SetId(id_manager->next(0));
    list_new_points.push_back(n);
  }

  NodeAdaptive *point_back = static_cast<NodeAdaptive *>(points.back());
  point_back->SetId(id_manager->next(0));
  list_new_points.push_back(point_back);

  return list_new_points;
}

SubMesh *Adapter::AdaptDomainOmp(PatchCoons *coons_patch,
                                 Performer::IdManager *id_manager,
                                 double factor_disc_global) {
  SubMesh *sub_mesh_new = new SubMesh;
  // pow ( 2, exp ); // o fator de rediscretização
  double factor_disc = DISCRETIZATION_CURVE_FACTOR_INTERNAL;
  // avanço ( proporção do triângulo, tolerância, número de vezes o refinamento)
  AdvancingFront avanco(RATIO_AFT, TOLERANCE_AFT, SMOOTHING_LAPLACIAN_NUMBER,
                        SMOOTHING_LAPLACIAN_FACTOR);
  map<Vertex *, NodeAdaptive *> map;

  // 1. Para cada curva do patch
  for (unsigned int i = 0; i < coons_patch->GetNumBerCurves(); ++i) {
    CurveAdaptive *curve = coons_patch->GetCurve(i);
    //((CurveAdaptiveParametric*)c)->ordenaLista ( );
    // #pragma omp critical
    //        {
    //            ((CurveAdaptiveParametric*)c)->ordenaLista ( );
    //        }

    if (i == 0 or i == 1) {
      list<double>::iterator last_parameter =
          static_cast<CurveAdaptiveParametric *>(curve)->parameters_.end();
      --last_parameter;

      int parameter = 0;

      for (auto param_iterator = static_cast<CurveAdaptiveParametric *>(curve)
                                     ->parameters_.begin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 0)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 0.0,
              static_cast<CurveAdaptiveParametric *>(curve));
        else if (i == 1)
          vertex = avanco.getBoundary()->addVertex(
              1.0, *param_iterator,
              static_cast<CurveAdaptiveParametric *>(curve));

        NodeAdaptive *noh =
            static_cast<NodeAdaptive *>(curve->GetPoint(parameter));

        map[vertex] = noh;

        sub_mesh_new->SetNoh(noh);

        parameter++;
      }
    } else if (i == 2 or i == 3) {
      list<double>::reverse_iterator last_parameter =
          static_cast<CurveAdaptiveParametric *>(curve)->parameters_.rend();
      --last_parameter;

      int parameter =
          static_cast<CurveAdaptiveParametric *>(curve)->GetNumBerPoints() - 1;

      for (list<double>::reverse_iterator param_iterator =
               (static_cast<CurveAdaptiveParametric *>(curve))
                   ->parameters_.rbegin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 2)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 1.0,
              static_cast<CurveAdaptiveParametric *>(curve));
        else if (i == 3)
          vertex = avanco.getBoundary()->addVertex(
              0.0, *param_iterator,
              static_cast<CurveAdaptiveParametric *>(curve));

        NodeAdaptive *noh =
            static_cast<NodeAdaptive *>(curve->GetPoint(parameter));

        map[vertex] = noh;

        sub_mesh_new->SetNoh(noh);

        parameter--;
      }
    }
  }

  avanco.getBoundary()->close(static_cast<CurveAdaptiveParametric *>(
      coons_patch->GetCurve(coons_patch->GetNumBerCurves() - 1)));
  // essa é a malha anterior!
  SubMesh *sub_mesh_old = coons_patch->GetSubMesh();

  // constroi a lista de triangulos antigos para o gerador de malha
  FaceList mesh_old;
  // área de todos os elementos
  double area_total = 0;

  for (unsigned int i = 0; i < sub_mesh_old->GetNumberElements(); ++i) {
    TriangleAdaptive *tri =
        static_cast<TriangleAdaptive *>(sub_mesh_old->GetElement(i));

    /*Noh centro (	( tri->GetNoh ( 1 ).x + tri->GetNoh ( 2 ).x +
tri->GetNoh
(
3
).x ) / 3.0 , ( tri->GetNoh ( 1 ).y + tri->GetNoh ( 2 ).y + tri->GetNoh ( 3 ).y
) / 3.0 , ( tri->GetNoh ( 1 ).z + tri->GetNoh ( 2 ).z + tri->GetNoh ( 3 ).z  )
/ 3.0
); tuple < double, double > centro_par = patch->encontrar_u_v ( centro );*/

    Vertex *v1 = new Vertex(get<0>(tri->GetParametersN1()),
                            get<1>(tri->GetParametersN1()));
    Vertex *v2 = new Vertex(get<0>(tri->GetParametersN2()),
                            get<1>(tri->GetParametersN2()));
    Vertex *v3 = new Vertex(get<0>(tri->GetParametersN3()),
                            get<1>(tri->GetParametersN3()));
    // Vertex* c = new Vertex  ( get<0>( centro_par ), get<1>( centro_par ) );

    // cout << "APC.cpp: u = " << get<0>( centro_par ) << " v = " << get<1>(
    // centro_par ) << endl;

    Face *face = new Face(v1, v2, v3, tri->GetId() /*, c*/);
    area_total += tri->GetArea();

    mesh_old.push_back(face);
  }

  FaceList::iterator face_list_iterator = mesh_old.begin();

  for (unsigned int i = 0; i < sub_mesh_old->GetNumberElements(); ++i) {
    double length_old = 0;

    TriangleAdaptive *tri =
        static_cast<TriangleAdaptive *>(sub_mesh_old->GetElement(i));
    Face *face = (*face_list_iterator);

    length_old = sqrt(tri->GetArea() / area_total);

    double lenght_new = 0.0;
    double ka = 0.0;
    double kd = 0.0;

    if (fabs(tri->GetNoh(1).GetGa()) >= TOLERANCE &&
        fabs(tri->GetNoh(2).GetGa()) >= TOLERANCE &&
        fabs(tri->GetNoh(3).GetGa()) >= TOLERANCE) {
      ka = (tri->GetNoh(1).GetGa() + tri->GetNoh(2).GetGa() +
            tri->GetNoh(3).GetGa()) /
           3.0;
      kd = (tri->GetNoh(1).GetGd() + tri->GetNoh(2).GetGd() +
            tri->GetNoh(3).GetGd()) /
           3.0;
    } else {
      ka = (tri->GetNoh(1).GetHa() + tri->GetNoh(2).GetHa() +
            tri->GetNoh(3).GetHa()) /
           3.0;
      kd = (tri->GetNoh(1).GetHd() + tri->GetNoh(2).GetHd() +
            tri->GetNoh(3).GetHd()) /
           3.0;
    }

    lenght_new = CalculateNewSize(ka, kd, factor_disc, length_old);

    // TODO(tsombra): Refina ou desrifina?

    face->h = lenght_new * factor_disc_global;

    face_list_iterator++;
  }

  // FaceList emptyMesh;

  if (!avanco.execute(mesh_old)) {
    VertexList vertices = avanco.getVertices();

    while (!vertices.empty()) {
      vertices.pop_front();
    }

    EdgeList edges = avanco.getEdges();

    while (!edges.empty()) {
      edges.pop_front();
    }

    FaceList faces = avanco.getMesh();

    while (!faces.empty()) {
      faces.pop_front();
    }

    exit(0);
  }

  // pega os triangulos gerados pelo avanco de fronteira
  FaceList new_mesh = avanco.getMesh();

  VertexList new_vertices = avanco.getInnerVertices();

  while (!new_vertices.empty()) {
    Vertex *vertex = new_vertices.front();
    new_vertices.pop_front();

    NodeAdaptive *noh = new NodeAdaptive(
        coons_patch->Parameterize(vertex->getX(), vertex->getY()));

    noh->SetId(id_manager->next(0));

    map[vertex] = noh;
    sub_mesh_new->SetNoh(noh);
  }

  // id_ele = 1;

  // gera os triangulos
  while (!new_mesh.empty()) {
    Face *face = new_mesh.front();
    new_mesh.pop_front();

    ElementAdaptive *element =
        new TriangleAdaptive(static_cast<NodeAdaptive *>(map[face->getV1()]),
                             static_cast<NodeAdaptive *>(map[face->getV2()]),
                             static_cast<NodeAdaptive *>(map[face->getV3()]));

    element->SetId(/*id_ele++*/ id_manager->next(1));

    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN1(
            make_tuple(face->getV1()->getX(), face->getV1()->getY()));
    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN2(
            make_tuple(face->getV2()->getX(), face->getV2()->getY()));
    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN3(
            make_tuple(face->getV3()->getX(), face->getV3()->getY()));

    sub_mesh_new->SetElement(element);
  }

  // apaga os triangulos da malha antiga
  while (!mesh_old.empty()) {
    Face *face = mesh_old.front();
    mesh_old.pop_front();

    delete face;
  }

  return sub_mesh_new;
}
#endif  // #USE_OPENMP

list<PointAdaptive *> Adapter::AdaptCurveByCurve(
    CurveAdaptive *curve, map<PointAdaptive *, PointAdaptive *> &map_points,
    Performer::IdManager *id_manager, double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<PointAdaptive *> points = curve->GetPoints();
  list<PointAdaptive *>::iterator point_current = points.begin();
  list<PointAdaptive *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  PointAdaptive midpoint;

  static_cast<CurveAdaptiveParametric *>(curve)->SortPointsByParameters();

  // 1.1. Inicialize a árvore binária com a raiz para toda a curva
  BinTree bin_tree;

  // 1.2. Para cada segmento da curva
  while (point_next != points.end()) {
    // o tamanho velho de cada segmento
    double length_old = 0;
    // o tamanho novo de cada segmento
    double lenght_new = 0;
    // tamanho paramétrico de cada segmento
    double lenght_par = 0;
    // parametro correspondente ao ponto central do segmento
    double midpoint_segment = 0;
    // pow ( 2, exp ); // o fator de rediscretização
    double factor_disc = DISCRETIZATION_CURVE_FACTOR;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old =
        curve->CalculateLengthPoints(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint =
        static_cast<CurveAdaptiveParametric *>(curve)
            ->CalculateMidpointByPoints(*(*point_current), *(*point_next));

    // 1.2.2.1 Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurveAdaptiveParametric *>(curve)->FindParameterByPoint(
            midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    ka_midpoint = curve->CalculateCurvature(midpoint_segment);

    kd_average =
        (curve->CalculateCurvature(0) + curve->CalculateCurvature(1)) / 2.0;

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->GetLength();

    bin_tree.Subdivide(midpoint_segment, lenght_par * factor_disc_global,
                       static_cast<CurveAdaptiveParametric *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.Restrict(static_cast<CurveAdaptiveParametric *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.Rediscretization();
  (static_cast<CurveAdaptiveParametric *>(curve))->UpdateParameters(parameters);

  list<PointAdaptive *> list_new_points;

  NodeAdaptive *point_front = static_cast<NodeAdaptive *>(points.front());
  point_front->SetId(id_manager->next(0));

  NodeAdaptive *point_back = static_cast<NodeAdaptive *>(points.back());
  point_back->SetId(id_manager->next(0));

  NodeAdaptive *new_noh1, *new_noh2;

  map<PointAdaptive *, PointAdaptive *>::iterator point_iterator =
      map_points.find(point_front);

  if (point_iterator == map_points.end()) {
    new_noh1 = new NodeAdaptive(*point_front);
    new_noh1->SetId(id_manager->next(0));
    map_points[point_front] = new_noh1;
  } else {
    new_noh1 = static_cast<NodeAdaptive *>(map_points[point_front]);
  }

  point_iterator = map_points.find(point_back);

  if (point_iterator == map_points.end()) {
    new_noh2 = new NodeAdaptive(*point_back);
    new_noh2->SetId(id_manager->next(0));
    map_points[point_back] = new_noh2;
  } else {
    new_noh2 = static_cast<NodeAdaptive *>(map_points[point_back]);
  }

  for (list<double>::iterator param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    NodeAdaptive *noh =
        new NodeAdaptive((static_cast<CurveAdaptiveParametric *>(curve))
                             ->FindPointByParameter((*param_iterator)));
    noh->SetId(id_manager->next(0));
    list_new_points.push_back(noh);
  }

  list_new_points.push_front(new_noh1);
  list_new_points.push_back(new_noh2);

  return list_new_points;
}

list<PointAdaptive *> Adapter::AdaptCurveBySurface(
    CurveAdaptive *curve, map<PointAdaptive *, PointAdaptive *> &map_points,
    Performer::IdManager *id_manager, double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<PointAdaptive *> points = curve->GetPoints();
  list<PointAdaptive *>::iterator point_current = points.begin();
  list<PointAdaptive *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  PointAdaptive midpoint;

  static_cast<CurveAdaptiveParametric *>(curve)->SortPointsByParameters();

  // 1.1. Inicialize a árvore binária com a raiz para toda a curva
  BinTree bin_tree;

  // 1.2. Para cada segmento da curva
  while (point_next != points.end()) {
    // o tamanho velho de cada segmento
    double length_old = 0;
    // o tamanho novo de cada segmento
    double lenght_new = 0;
    // tamanho paramétrico de cada segmento
    double lenght_par = 0;
    // parametro correspondente ao ponto central do segmento
    double midpoint_segment = 0;
    // pow ( 2, exp ); // o fator de rediscretização
    double factor_disc = DISCRETIZATION_CURVE_FACTOR;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old =
        curve->CalculateLengthPoints(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint =
        static_cast<CurveAdaptiveParametric *>(curve)
            ->CalculateMidpointByPoints(*(*point_current), *(*point_next));

    // Teste para curvatura com a curva
    midpoint_segment =
        static_cast<CurveAdaptiveParametric *>(curve)->FindParameterByPoint(
            midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    CurvatureAnalytical ka_p0(midpoint,
                              *(static_cast<PatchCoons *>(curve->GetPatch(0))));

    for (unsigned int i = 0; i < curve->GetNumBerPatches(); i++) {
      CurvatureAnalytical ka_p1(
          midpoint, *(static_cast<PatchCoons *>(curve->GetPatch(i))));
      double Ga_p0 = ka_p0.CalculateGaussCurvature();
      double Ga_p1 = ka_p1.CalculateGaussCurvature();

      ka_midpoint = (fabs(Ga_p0) > fabs(Ga_p1)) ? Ga_p0 : Ga_p1;

      // testamos se ka é ZERO!
      if (fabs(ka_midpoint) < TOLERANCE) {
        double Ha_p0 = ka_p0.CalculateMeanCurvature();
        double Ha_p1 = ka_p1.CalculateMeanCurvature();

        ka_midpoint = (Ha_p0 > Ha_p1) ? Ha_p0 : Ha_p1;
        kd_average = (static_cast<NodeAdaptive *>((*point_current))->GetHd() +
                      static_cast<NodeAdaptive *>((*point_next))->GetHd()) /
                     2.0;
      } else {
        kd_average = (static_cast<NodeAdaptive *>((*point_current))->GetGd() +
                      static_cast<NodeAdaptive *>((*point_next))->GetGd()) /
                     2.0;
      }
    }

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->GetLength();

    // 1.2.6. Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurveAdaptiveParametric *>(curve)->FindParameterByPoint(
            midpoint);

    bin_tree.Subdivide(midpoint_segment, lenght_par * factor_disc_global,
                       static_cast<CurveAdaptiveParametric *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.Restrict(static_cast<CurveAdaptiveParametric *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.Rediscretization();
  (static_cast<CurveAdaptiveParametric *>(curve))->UpdateParameters(parameters);

  list<PointAdaptive *> list_new_points;

  NodeAdaptive *point_front = static_cast<NodeAdaptive *>(points.front());
  point_front->SetId(id_manager->next(0));
  NodeAdaptive *point_back = static_cast<NodeAdaptive *>(points.back());
  point_back->SetId(id_manager->next(0));

  NodeAdaptive *new_noh1, *new_noh2;

  map<PointAdaptive *, PointAdaptive *>::iterator point_iterator =
      map_points.find(point_front);

  if (point_iterator == map_points.end()) {
    new_noh1 = new NodeAdaptive(*point_front);
    new_noh1->SetId(id_manager->next(0));
    map_points[point_front] = new_noh1;
  } else {
    new_noh1 = static_cast<NodeAdaptive *>(map_points[point_front]);
  }

  point_iterator = map_points.find(point_back);

  if (point_iterator == map_points.end()) {
    new_noh2 = new NodeAdaptive(*point_back);
    new_noh2->SetId(id_manager->next(0));
    map_points[point_back] = new_noh2;
  } else {
    new_noh2 = static_cast<NodeAdaptive *>(map_points[point_back]);
  }

  for (list<double>::iterator param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    NodeAdaptive *noh =
        new NodeAdaptive((static_cast<CurveAdaptiveParametric *>(curve))
                             ->FindPointByParameter((*param_iterator)));
    noh->SetId(id_manager->next(0));
    list_new_points.push_back(noh);
  }

  list_new_points.push_front(new_noh1);
  list_new_points.push_back(new_noh2);

  return list_new_points;
}

// Usa a QuadTree. Gera uma nova malha e atualiza a malha do patch. A malha
// gerada deve ser inserida no modelo pelo Gerador Adaptativo
SubMesh *Adapter::AdaptDomain(PatchCoons *coons_patch,
                              Performer::IdManager *id_manager,
                              double factor_disc_global) {
  SubMesh *sub_mesh_new = new SubMesh;
  double factor_disc =
      DISCRETIZATION_CURVE_FACTOR_INTERNAL;  // pow ( 2, exp ); // o fator de
                                             // rediscretização

  // avanço ( proporção do triângulo, tolerância, número de vezes o refinamento)
  AdvancingFront avanco(RATIO_AFT, TOLERANCE_AFT, SMOOTHING_LAPLACIAN_NUMBER,
                        SMOOTHING_LAPLACIAN_FACTOR);
  map<Vertex *, NodeAdaptive *> map;

  // 1. Para cada curva do patch
  for (unsigned int i = 0; i < coons_patch->GetNumBerCurves(); ++i) {
    CurveAdaptive *curve = coons_patch->GetCurve(i);
    (static_cast<CurveAdaptiveParametric *>(curve))->SortPointsByParameters();

    if (i == 0 or i == 1) {
      list<double>::iterator last_parameter =
          (static_cast<CurveAdaptiveParametric *>(curve))->parameters_.end();
      --last_parameter;

      int parameter = 0;

      for (auto param_iterator = static_cast<CurveAdaptiveParametric *>(curve)
                                     ->parameters_.begin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 0)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 0.0,
              static_cast<CurveAdaptiveParametric *>(curve));
        else if (i == 1)
          vertex = avanco.getBoundary()->addVertex(
              1.0, *param_iterator,
              static_cast<CurveAdaptiveParametric *>(curve));

        NodeAdaptive *noh =
            static_cast<NodeAdaptive *>(curve->GetPoint(parameter));

        map[vertex] = noh;

        sub_mesh_new->SetNoh(noh);

        parameter++;
      }
    } else if (i == 2 or i == 3) {
      list<double>::reverse_iterator last_parameter =
          (static_cast<CurveAdaptiveParametric *>(curve))->parameters_.rend();
      --last_parameter;

      int parameter =
          static_cast<CurveAdaptiveParametric *>(curve)->GetNumBerPoints() - 1;

      for (list<double>::reverse_iterator param_iterator =
               (static_cast<CurveAdaptiveParametric *>(curve))
                   ->parameters_.rbegin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 2)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 1.0,
              static_cast<CurveAdaptiveParametric *>(curve));
        else if (i == 3)
          vertex = avanco.getBoundary()->addVertex(
              0.0, *param_iterator,
              static_cast<CurveAdaptiveParametric *>(curve));

        NodeAdaptive *noh =
            static_cast<NodeAdaptive *>(curve->GetPoint(parameter));

        map[vertex] = noh;

        sub_mesh_new->SetNoh(noh);

        parameter--;
      }
    }
  }

  avanco.getBoundary()->close(static_cast<CurveAdaptiveParametric *>(
      coons_patch->GetCurve(coons_patch->GetNumBerCurves() - 1)));

  SubMesh *sub_mesh_old =
      coons_patch->GetSubMesh();  // essa é a malha anterior!

  // constroi a lista de triangulos antigos para o gerador de malha
  FaceList mesh_old;

  double area_total = 0;  // área de todos os elementos

  for (unsigned int i = 0; i < sub_mesh_old->GetNumberElements(); ++i) {
    TriangleAdaptive *tri =
        static_cast<TriangleAdaptive *>(sub_mesh_old->GetElement(i));

    /*Noh centro (	( tri->GetNoh ( 1 ).x + tri->GetNoh ( 2 ).x +
tri->GetNoh
(
3
).x ) / 3.0 , ( tri->GetNoh ( 1 ).y + tri->GetNoh ( 2 ).y + tri->GetNoh ( 3 ).y
) / 3.0 , ( tri->GetNoh ( 1 ).z + tri->GetNoh ( 2 ).z + tri->GetNoh ( 3 ).z  )
/ 3.0
); tuple < double, double > centro_par = patch->encontrar_u_v ( centro );*/

    Vertex *v1 = new Vertex(get<0>(tri->GetParametersN1()),
                            get<1>(tri->GetParametersN1()));
    Vertex *v2 = new Vertex(get<0>(tri->GetParametersN2()),
                            get<1>(tri->GetParametersN2()));
    Vertex *v3 = new Vertex(get<0>(tri->GetParametersN3()),
                            get<1>(tri->GetParametersN3()));
    // Vertex* c = new Vertex  ( get<0>( centro_par ), get<1>( centro_par ) );

    // cout << "APC.cpp: u = " << get<0>( centro_par ) << " v = " << get<1>(
    // centro_par ) << endl;

    Face *face = new Face(v1, v2, v3, tri->GetId() /*, c*/);
    area_total += tri->GetArea();

    mesh_old.push_back(face);
  }

  FaceList::iterator face_list_iterator = mesh_old.begin();

  for (unsigned int i = 0; i < sub_mesh_old->GetNumberElements(); ++i) {
    double length_old = 0;

    TriangleAdaptive *tri =
        static_cast<TriangleAdaptive *>(sub_mesh_old->GetElement(i));
    Face *face = (*face_list_iterator);

    length_old = sqrt(tri->GetArea() / area_total);

    double lenght_new = 0.0;
    double ka = 0.0;
    double kd = 0.0;

    if (fabs(tri->GetNoh(1).GetGa()) >= TOLERANCE &&
        fabs(tri->GetNoh(2).GetGa()) >= TOLERANCE &&
        fabs(tri->GetNoh(3).GetGa()) >= TOLERANCE) {
      ka = (tri->GetNoh(1).GetGa() + tri->GetNoh(2).GetGa() +
            tri->GetNoh(3).GetGa()) /
           3.0;
      kd = (tri->GetNoh(1).GetGd() + tri->GetNoh(2).GetGd() +
            tri->GetNoh(3).GetGd()) /
           3.0;
    } else {
      ka = (tri->GetNoh(1).GetHa() + tri->GetNoh(2).GetHa() +
            tri->GetNoh(3).GetHa()) /
           3.0;
      kd = (tri->GetNoh(1).GetHd() + tri->GetNoh(2).GetHd() +
            tri->GetNoh(3).GetHd()) /
           3.0;
    }

    lenght_new = CalculateNewSize(ka, kd, factor_disc, length_old);

    /*if ( h_novo > h_velho )
    cout << "desrefina" << endl;
else
    cout << "refina" << endl;*/

    face->h = lenght_new * factor_disc_global;

    face_list_iterator++;
  }

  FaceList emptyMesh;
  if (!avanco.execute(mesh_old)) {
    VertexList vertices = avanco.getVertices();

    while (!vertices.empty()) {
      vertices.pop_front();
    }

    EdgeList edges = avanco.getEdges();

    while (!edges.empty()) {
      edges.pop_front();
    }

    FaceList faces = avanco.getMesh();

    while (!faces.empty()) {
      faces.pop_front();
    }

    exit(0);
  }

  // pega os triangulos gerados pelo avanco de fronteira
  FaceList new_mesh = avanco.getMesh();

  VertexList new_vertices = avanco.getInnerVertices();

  while (!new_vertices.empty()) {
    Vertex *vertex = new_vertices.front();
    new_vertices.pop_front();

    NodeAdaptive *noh = new NodeAdaptive(
        coons_patch->Parameterize(vertex->getX(), vertex->getY()));

    noh->SetId(id_manager->next(0));

    map[vertex] = noh;
    sub_mesh_new->SetNoh(noh);
  }

  // id_ele = 1;

  // gera os triangulos
  while (!new_mesh.empty()) {
    Face *face = new_mesh.front();
    new_mesh.pop_front();

    ElementAdaptive *element =
        new TriangleAdaptive(static_cast<NodeAdaptive *>(map[face->getV1()]),
                             static_cast<NodeAdaptive *>(map[face->getV2()]),
                             static_cast<NodeAdaptive *>(map[face->getV3()]));

    element->SetId(id_manager->next(1));

    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN1(
            make_tuple(face->getV1()->getX(), face->getV1()->getY()));
    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN2(
            make_tuple(face->getV2()->getX(), face->getV2()->getY()));
    (static_cast<TriangleAdaptive *>(element))
        ->SetParametersN3(
            make_tuple(face->getV3()->getX(), face->getV3()->getY()));

    sub_mesh_new->SetElement(element);
  }

  // apaga os triangulos da malha antiga
  while (!mesh_old.empty()) {
    Face *face = mesh_old.front();
    mesh_old.pop_front();

    delete face;
  }

  return sub_mesh_new;
}

double Adapter::CalculateNewSize(const double ka, const double kd,
                                 const double factor_disc,
                                 const double length_old) {
  // Cenário 1 : ka está muito próxima a kd
  if (((ka - TOLERANCE_CURVATURE) < kd) && (kd < (ka + TOLERANCE_CURVATURE))) {
    // está próximo ao plano, desrefine
    if (fabs(ka) < TOLERANCE_CURVATURE) {
      return length_old * factor_disc;
    } else {
      // a discretização reflete bem a superfície, não faça nada!
      return length_old;
    }
  }
  // Outros cenários : refine
  else {
    return length_old / factor_disc;
  }
}

// #pragma omp parallel for num_threads(NUM_THREADS) firstprivate(atual,proxi)
//  shared(bt)
//     // 1.2. Para cada segmento da curva
//     for (int i = 0; i < pontos.size(); ++i)
//     {
//         // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
//         h_velho = c->CalculateLengthPoint ( *(*atual), *(*proxi) );

//        // cout << "tamanho do segmento de curva"<<h_velho<< endl;

//        //cout << "calculou novo tamanho" << endl;

//        // 1.2.2. Calcule o ponto médio do segmento
//        C_seg = static_cast < CurveAdaptiveParametric* > ( c )->pontoMedio (
//        *(*atual), *(*proxi) );

//        // 1.2.2.1 Encontre o parâmetro do ponto médio
//        t = static_cast < CurveAdaptiveParametric* > ( c )->encontrar_t (
//        C_seg );

//        // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio

//        ka = c->CalculateCurvature(t);

//        kd = (c->CalculateCurvature(0) + c->CalculateCurvature(1)) / 2.0;

//        // 1.2.4. O novo tamanho é calculado de acordo com os cenários
//        h_novo = novoTamanho ( ka, kd, f, h_velho );

//        //cout << "calculou novo tamanho" << endl;

//        // 1.2.5. Calcule o tamanho paramétrico
//        h_par = h_novo / c->get_L ( );

//        bt.subdividir( t, h_par*fator_dis, (CurveAdaptiveParametric*)c );

//        ++proxi;
//        ++atual;
//    }
