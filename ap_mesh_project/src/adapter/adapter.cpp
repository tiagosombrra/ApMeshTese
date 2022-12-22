#include "../../include/adapter/adapter.h"

extern double TOLERANCIA;
extern double TOLERANCIA_AFT;
extern double TOLERANCIA_CURVATURA;
extern double PROPORCAO;
extern double SUAVIZACAO;
extern double FATOR_SUAVIZACAO;
extern double DISCRETIZACAO_CURVA;
extern double DISCRETIZACAO_INTER;

#if USE_OPENMP
list<Ponto *> Adapter::AdaptCurveByCurveOmp(Curva *curve,
                                            Performer::IdManager *id_manager,
                                            double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<Ponto *> points = curve->getPontos();
  list<Ponto *>::iterator point_current = points.begin();
  list<Ponto *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  Ponto midpoint;

  static_cast<CurvaParametrica *>(curve)->ordenaLista();

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
    double factor_disc = DISCRETIZACAO_CURVA;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old = curve->calcularTamanho(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint = static_cast<CurvaParametrica *>(curve)->pontoMedio(
        *(*point_current), *(*point_next));
    midpoint.id = id_manager->next(0);

    // 1.2.2.1 Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurvaParametrica *>(curve)->encontrar_t(midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    ka_midpoint = curve->calcularCurvatura(midpoint_segment);

    kd_average =
        (curve->calcularCurvatura(0) + curve->calcularCurvatura(1)) / 2.0;

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->get_L();

    bin_tree.subdividir(midpoint_segment, lenght_par * factor_disc_global,
                        static_cast<CurvaParametrica *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.restringir(static_cast<CurvaParametrica *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.rediscretizacao();
  (static_cast<CurvaParametrica *>(curve))->atualizarParametros(parameters);

  list<Ponto *> list_new_points;

  Noh *point_front = static_cast<Noh *>(points.front());
  point_front->id = id_manager->next(0);

  Noh *point_back = static_cast<Noh *>(points.back());
  point_back->id = id_manager->next(0);

  list_new_points.push_front(point_front);

  for (auto param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    Noh *point_intermediate = new Noh((static_cast<CurvaParametrica *>(curve))
                                          ->parametrizar((*param_iterator)));
    point_intermediate->id = id_manager->next(0);
    list_new_points.push_back(point_intermediate);
  }

  list_new_points.push_back(point_back);

  return list_new_points;

  //    for (list<double>::iterator it = ++parametros.begin();
  //         it != --parametros.end(); it++)
  //    {
  //        Noh *n = new Noh(((CurvaParametrica *)c)->parametrizar((*it)));
  //        n->id = idManager->next(0);
  //        c->inserePonto(n);
  //    }

  //    return c;
}

list<Ponto *> Adapter::AdaptCurveBySurfaceOmp(Curva *curve,
                                              Performer::IdManager *id_manager,
                                              double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<Ponto *> points = curve->getPontos();
  list<Ponto *>::iterator point_current = points.begin();
  list<Ponto *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  Ponto midpoint;

  static_cast<CurvaParametrica *>(curve)->ordenaLista();

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
    double factor_disc = DISCRETIZACAO_CURVA;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old = curve->calcularTamanho(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint = static_cast<CurvaParametrica *>(curve)->pontoMedio(
        *(*point_current), *(*point_next));

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    CurvaturaAnalitica ka_p0(midpoint,
                             *(static_cast<CoonsPatch *>(curve->getPatch(0))));
    ka_midpoint = ka_p0.gauss();

    if (curve->getNumDePatches() == 1) {
      // testamos se ka é ZERO!
      if (fabs(ka_midpoint) < TOLERANCIA) {
        ka_midpoint = ka_p0.media();
        kd_average = ((static_cast<Noh *>((*point_current)))->Hd +
                      (static_cast<Noh *>((*point_next))->Hd)) /
                     2.0;
      } else {
        kd_average = ((static_cast<Noh *>((*point_current)))->Gd +
                      (static_cast<Noh *>((*point_next)))->Gd) /
                     2.0;
      }
    } else {
      double Ha = ka_p0.media();

      for (unsigned int i = 1; i < curve->getNumDePatches(); i++) {
        CurvaturaAnalitica ka_pi(
            midpoint, *(static_cast<CoonsPatch *>(curve->getPatch(i))));
        double Ga_pi = ka_pi.gauss();

        ka_midpoint = (fabs(ka_midpoint) > fabs(Ga_pi)) ? ka_midpoint : Ga_pi;

        // testamos se ka é ZERO!
        if (fabs(ka_midpoint) < TOLERANCIA) {
          double Ha_pi = ka_pi.media();

          ka_midpoint = (Ha > Ha_pi) ? Ha : Ha_pi;
          kd_average = ((static_cast<Noh *>((*point_current)))->Hd +
                        (static_cast<Noh *>((*point_next))->Hd)) /
                       2.0;
        } else {
          kd_average = ((static_cast<Noh *>((*point_current)))->Gd +
                        (static_cast<Noh *>((*point_next)))->Gd) /
                       2.0;
        }
      }
    }

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->get_L();

    // 1.2.6. Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurvaParametrica *>(curve)->encontrar_t(midpoint);

    bin_tree.subdividir(midpoint_segment, lenght_par * factor_disc_global,
                        static_cast<CurvaParametrica *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.restringir(static_cast<CurvaParametrica *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.rediscretizacao();
  (static_cast<CurvaParametrica *>(curve))->atualizarParametros(parameters);

  list<Ponto *> list_new_points;

  Noh *point_front = static_cast<Noh *>(points.front());
  point_front->id = id_manager->next(0);
  list_new_points.push_front(point_front);

  for (auto param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    Noh *n = new Noh((static_cast<CurvaParametrica *>(curve))
                         ->parametrizar((*param_iterator)));
    n->id = id_manager->next(0);
    list_new_points.push_back(n);
  }

  Noh *point_back = static_cast<Noh *>(points.back());
  point_back->id = id_manager->next(0);
  list_new_points.push_back(point_back);

  return list_new_points;
}

SubMalha *Adapter::AdaptDomainOmp(CoonsPatch *coons_patch,
                                  Performer::IdManager *id_manager,
                                  double factor_disc_global) {
  SubMalha *sub_mesh_new = new SubMalha;
  // pow ( 2, exp ); // o fator de rediscretização
  double factor_disc = DISCRETIZACAO_INTER;
  // avanço ( proporção do triângulo, tolerância, número de vezes o refinamento)
  AdvancingFront avanco(PROPORCAO, TOLERANCIA_AFT, SUAVIZACAO,
                        FATOR_SUAVIZACAO);
  map<Vertex *, Noh *> map;

  // 1. Para cada curva do patch
  for (unsigned int i = 0; i < coons_patch->getNumDeCurvas(); ++i) {
    Curva *curve = coons_patch->getCurva(i);
    //((CurvaParametrica*)c)->ordenaLista ( );
    // #pragma omp critical
    //        {
    //            ((CurvaParametrica*)c)->ordenaLista ( );
    //        }

    if (i == 0 or i == 1) {
      list<double>::iterator last_parameter =
          static_cast<CurvaParametrica *>(curve)->parametros.end();
      --last_parameter;

      int parameter = 0;

      for (auto param_iterator =
               static_cast<CurvaParametrica *>(curve)->parametros.begin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 0)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 0.0, static_cast<CurvaParametrica *>(curve));
        else if (i == 1)
          vertex = avanco.getBoundary()->addVertex(
              1.0, *param_iterator, static_cast<CurvaParametrica *>(curve));

        Noh *noh = static_cast<Noh *>(curve->getPonto(parameter));

        map[vertex] = noh;

        sub_mesh_new->insereNoh(noh);

        parameter++;
      }
    } else if (i == 2 or i == 3) {
      list<double>::reverse_iterator last_parameter =
          static_cast<CurvaParametrica *>(curve)->parametros.rend();
      --last_parameter;

      int parameter =
          static_cast<CurvaParametrica *>(curve)->getNumDePontos() - 1;

      for (list<double>::reverse_iterator param_iterator =
               (static_cast<CurvaParametrica *>(curve))->parametros.rbegin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 2)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 1.0, static_cast<CurvaParametrica *>(curve));
        else if (i == 3)
          vertex = avanco.getBoundary()->addVertex(
              0.0, *param_iterator, static_cast<CurvaParametrica *>(curve));

        Noh *noh = static_cast<Noh *>(curve->getPonto(parameter));

        map[vertex] = noh;

        sub_mesh_new->insereNoh(noh);

        parameter--;
      }
    }
  }

  avanco.getBoundary()->close(static_cast<CurvaParametrica *>(
      coons_patch->getCurva(coons_patch->getNumDeCurvas() - 1)));
  // essa é a malha anterior!
  SubMalha *sub_mesh_old = coons_patch->getMalha();

  // constroi a lista de triangulos antigos para o gerador de malha
  FaceList mesh_old;
  // área de todos os elementos
  double area_total = 0;

  for (unsigned int i = 0; i < sub_mesh_old->getNumDeElementos(); ++i) {
    Triangulo *tri = static_cast<Triangulo *>(sub_mesh_old->getElemento(i));

    /*Noh centro (	( tri->getN ( 1 ).x + tri->getN ( 2 ).x + tri->getN ( 3
).x ) / 3.0 , ( tri->getN ( 1 ).y + tri->getN ( 2 ).y + tri->getN ( 3 ).y  )
/ 3.0 , ( tri->getN ( 1 ).z + tri->getN ( 2 ).z + tri->getN ( 3 ).z  ) / 3.0
); tuple < double, double > centro_par = patch->encontrar_u_v ( centro );*/

    Vertex *v1 = new Vertex(get<0>(tri->p1), get<1>(tri->p1));
    Vertex *v2 = new Vertex(get<0>(tri->p2), get<1>(tri->p2));
    Vertex *v3 = new Vertex(get<0>(tri->p3), get<1>(tri->p3));
    // Vertex* c = new Vertex  ( get<0>( centro_par ), get<1>( centro_par ) );

    // cout << "APC.cpp: u = " << get<0>( centro_par ) << " v = " << get<1>(
    // centro_par ) << endl;

    Face *face = new Face(v1, v2, v3, tri->getId() /*, c*/);
    area_total += tri->getArea();

    mesh_old.push_back(face);
  }

  FaceList::iterator face_list_iterator = mesh_old.begin();

  for (unsigned int i = 0; i < sub_mesh_old->getNumDeElementos(); ++i) {
    double length_old = 0;

    Triangulo *tri = static_cast<Triangulo *>(sub_mesh_old->getElemento(i));
    Face *face = (*face_list_iterator);

    length_old = sqrt(tri->getArea() / area_total);

    double lenght_new = 0.0;
    double ka = 0.0;
    double kd = 0.0;

    if (fabs(tri->getN(1).Ga) >= TOLERANCIA and
        fabs(tri->getN(2).Ga) >= TOLERANCIA and
        fabs(tri->getN(3).Ga) >= TOLERANCIA) {
      ka = (tri->getN(1).Ga + tri->getN(2).Ga + tri->getN(3).Ga) / 3.0;
      kd = (tri->getN(1).Gd + tri->getN(2).Gd + tri->getN(3).Gd) / 3.0;
    } else {
      ka = (tri->getN(1).Ha + tri->getN(2).Ha + tri->getN(3).Ha) / 3.0;
      kd = (tri->getN(1).Hd + tri->getN(2).Hd + tri->getN(3).Hd) / 3.0;
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

    Noh *noh =
        new Noh(coons_patch->parametrizar(vertex->getX(), vertex->getY()));

    noh->id = /*id_noh++*/ id_manager->next(0);

    map[vertex] = noh;
    sub_mesh_new->insereNoh(noh);
  }

  // id_ele = 1;

  // gera os triangulos
  while (!new_mesh.empty()) {
    Face *face = new_mesh.front();
    new_mesh.pop_front();

    Elemento *element = new Triangulo(static_cast<Noh *>(map[face->getV1()]),
                                      static_cast<Noh *>(map[face->getV2()]),
                                      static_cast<Noh *>(map[face->getV3()]));

    element->setId(/*id_ele++*/ id_manager->next(1));

    (static_cast<Triangulo *>(element))->p1 =
        make_tuple(face->getV1()->getX(), face->getV1()->getY());
    (static_cast<Triangulo *>(element))->p2 =
        make_tuple(face->getV2()->getX(), face->getV2()->getY());
    (static_cast<Triangulo *>(element))->p3 =
        make_tuple(face->getV3()->getX(), face->getV3()->getY());

    sub_mesh_new->insereElemento(element);
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

list<Ponto *> Adapter::AdaptCurveByCurve(Curva *curve,
                                         map<Ponto *, Ponto *> &map_points,
                                         Performer::IdManager *id_manager,
                                         double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<Ponto *> points = curve->getPontos();
  list<Ponto *>::iterator point_current = points.begin();
  list<Ponto *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  Ponto midpoint;

  static_cast<CurvaParametrica *>(curve)->ordenaLista();

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
    double factor_disc = DISCRETIZACAO_CURVA;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old = curve->calcularTamanho(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint = static_cast<CurvaParametrica *>(curve)->pontoMedio(
        *(*point_current), *(*point_next));

    // 1.2.2.1 Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurvaParametrica *>(curve)->encontrar_t(midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    ka_midpoint = curve->calcularCurvatura(midpoint_segment);

    kd_average =
        (curve->calcularCurvatura(0) + curve->calcularCurvatura(1)) / 2.0;

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->get_L();

    bin_tree.subdividir(midpoint_segment, lenght_par * factor_disc_global,
                        static_cast<CurvaParametrica *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.restringir(static_cast<CurvaParametrica *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.rediscretizacao();
  (static_cast<CurvaParametrica *>(curve))->atualizarParametros(parameters);

  list<Ponto *> list_new_points;

  Noh *point_front = static_cast<Noh *>(points.front());
  point_front->id = id_manager->next(0);

  Noh *point_back = static_cast<Noh *>(points.back());
  point_back->id = id_manager->next(0);

  Noh *new_noh1, *new_noh2;

  map<Ponto *, Ponto *>::iterator point_iterator = map_points.find(point_front);

  if (point_iterator == map_points.end()) {
    new_noh1 = new Noh(*point_front);
    new_noh1->id = id_manager->next(0);
    map_points[point_front] = new_noh1;
  } else {
    new_noh1 = static_cast<Noh *>(map_points[point_front]);
  }

  point_iterator = map_points.find(point_back);

  if (point_iterator == map_points.end()) {
    new_noh2 = new Noh(*point_back);
    new_noh2->id = id_manager->next(0);
    map_points[point_back] = new_noh2;
  } else {
    new_noh2 = static_cast<Noh *>(map_points[point_back]);
  }

  for (list<double>::iterator param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    Noh *noh = new Noh((static_cast<CurvaParametrica *>(curve))
                           ->parametrizar((*param_iterator)));
    noh->id = id_manager->next(0);
    list_new_points.push_back(noh);
  }

  list_new_points.push_front(new_noh1);
  list_new_points.push_back(new_noh2);

  return list_new_points;
}

list<Ponto *> Adapter::AdaptCurveBySurface(Curva *curve,
                                           map<Ponto *, Ponto *> &map_points,
                                           Performer::IdManager *id_manager,
                                           double factor_disc_global) {
  // os parametros gerados na rediscretização
  list<double> parameters;
  // os pontos da curva
  list<Ponto *> points = curve->getPontos();
  list<Ponto *>::iterator point_current = points.begin();
  list<Ponto *>::iterator point_next = points.begin();
  ++point_next;
  // ponto médio do segmentos
  Ponto midpoint;

  static_cast<CurvaParametrica *>(curve)->ordenaLista();

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
    double factor_disc = DISCRETIZACAO_CURVA;
    // curvatura analítica do ponto central de um segmento
    double ka_midpoint = 0;
    // média das curvaturas discretas dos extremos de um segmento
    double kd_average = 0;

    // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
    length_old = curve->calcularTamanho(*(*point_current), *(*point_next));

    // 1.2.2. Calcule o ponto médio do segmento
    midpoint = static_cast<CurvaParametrica *>(curve)->pontoMedio(
        *(*point_current), *(*point_next));

    // Teste para curvatura com a curva
    midpoint_segment =
        static_cast<CurvaParametrica *>(curve)->encontrar_t(midpoint);

    // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio
    CurvaturaAnalitica ka_p0(midpoint,
                             *(static_cast<CoonsPatch *>(curve->getPatch(0))));

    for (unsigned int i = 0; i < curve->getNumDePatches(); i++) {
      CurvaturaAnalitica ka_p1(
          midpoint, *(static_cast<CoonsPatch *>(curve->getPatch(i))));
      double Ga_p0 = ka_p0.gauss();
      double Ga_p1 = ka_p1.gauss();

      ka_midpoint = (fabs(Ga_p0) > fabs(Ga_p1)) ? Ga_p0 : Ga_p1;

      // testamos se ka é ZERO!
      if (fabs(ka_midpoint) < TOLERANCIA) {
        double Ha_p0 = ka_p0.media();
        double Ha_p1 = ka_p1.media();

        ka_midpoint = (Ha_p0 > Ha_p1) ? Ha_p0 : Ha_p1;
        kd_average = (static_cast<Noh *>((*point_current))->Hd +
                      static_cast<Noh *>((*point_next))->Hd) /
                     2.0;
      } else {
        kd_average = (static_cast<Noh *>((*point_current))->Gd +
                      static_cast<Noh *>((*point_next))->Gd) /
                     2.0;
      }
    }

    // 1.2.4. O novo tamanho é calculado de acordo com os cenários
    lenght_new =
        CalculateNewSize(ka_midpoint, kd_average, factor_disc, length_old);

    // 1.2.5. Calcule o tamanho paramétrico
    lenght_par = lenght_new / curve->get_L();

    // 1.2.6. Encontre o parâmetro do ponto médio
    midpoint_segment =
        static_cast<CurvaParametrica *>(curve)->encontrar_t(midpoint);

    bin_tree.subdividir(midpoint_segment, lenght_par * factor_disc_global,
                        static_cast<CurvaParametrica *>(curve));

    ++point_next;
    ++point_current;
  }

  // transforma a bintree numa bintree restrita
  while (bin_tree.restringir(static_cast<CurvaParametrica *>(curve))) {
  }

  // 1.3. Atualiza a lista de pontos da curva de acordo com as folhas da
  // BinTree
  parameters = bin_tree.rediscretizacao();
  (static_cast<CurvaParametrica *>(curve))->atualizarParametros(parameters);

  list<Ponto *> list_new_points;

  Noh *point_front = static_cast<Noh *>(points.front());
  point_front->id = id_manager->next(0);
  Noh *point_back = static_cast<Noh *>(points.back());
  point_back->id = id_manager->next(0);

  Noh *new_noh1, *new_noh2;

  map<Ponto *, Ponto *>::iterator point_iterator = map_points.find(point_front);

  if (point_iterator == map_points.end()) {
    new_noh1 = new Noh(*point_front);
    new_noh1->id = id_manager->next(0);
    map_points[point_front] = new_noh1;
  } else {
    new_noh1 = static_cast<Noh *>(map_points[point_front]);
  }

  point_iterator = map_points.find(point_back);

  if (point_iterator == map_points.end()) {
    new_noh2 = new Noh(*point_back);
    new_noh2->id = id_manager->next(0);
    map_points[point_back] = new_noh2;
  } else {
    new_noh2 = static_cast<Noh *>(map_points[point_back]);
  }

  for (list<double>::iterator param_iterator = ++parameters.begin();
       param_iterator != --parameters.end(); param_iterator++) {
    Noh *noh = new Noh((static_cast<CurvaParametrica *>(curve))
                           ->parametrizar((*param_iterator)));
    noh->id = id_manager->next(0);
    list_new_points.push_back(noh);
  }

  list_new_points.push_front(new_noh1);
  list_new_points.push_back(new_noh2);

  return list_new_points;
}

double Adapter::CalculateNewSize(const double ka, const double kd,
                                 const double factor_disc,
                                 const double length_old) {
  // Cenário 1 : ka está muito próxima a kd
  if (((ka - TOLERANCIA_CURVATURA) < kd) and
      (kd < (ka + TOLERANCIA_CURVATURA))) {
    // está próximo ao plano, desrefine
    if (fabs(ka) < TOLERANCIA_CURVATURA) {
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

// Usa a QuadTree. Gera uma nova malha e atualiza a malha do patch. A malha
// gerada deve ser inserida no modelo pelo Gerador Adaptativo
SubMalha *Adapter::AdaptDomain(CoonsPatch *coons_patch,
                               Performer::IdManager *id_manager,
                               double factor_disc_global) {
  SubMalha *sub_mesh_new = new SubMalha;
  double factor_disc =
      DISCRETIZACAO_INTER;  // pow ( 2, exp ); // o fator de rediscretização

  // avanço ( proporção do triângulo, tolerância, número de vezes o refinamento)
  AdvancingFront avanco(PROPORCAO, TOLERANCIA_AFT, SUAVIZACAO,
                        FATOR_SUAVIZACAO);
  map<Vertex *, Noh *> map;

  // 1. Para cada curva do patch
  for (unsigned int i = 0; i < coons_patch->getNumDeCurvas(); ++i) {
    Curva *curve = coons_patch->getCurva(i);
    (static_cast<CurvaParametrica *>(curve))->ordenaLista();

    if (i == 0 or i == 1) {
      list<double>::iterator last_parameter =
          (static_cast<CurvaParametrica *>(curve))->parametros.end();
      --last_parameter;

      int parameter = 0;

      for (auto param_iterator =
               (static_cast<CurvaParametrica *>(curve))->parametros.begin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 0)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 0.0, static_cast<CurvaParametrica *>(curve));
        else if (i == 1)
          vertex = avanco.getBoundary()->addVertex(
              1.0, *param_iterator, static_cast<CurvaParametrica *>(curve));

        Noh *noh = static_cast<Noh *>(curve->getPonto(parameter));

        map[vertex] = noh;

        sub_mesh_new->insereNoh(noh);

        parameter++;
      }
    } else if (i == 2 or i == 3) {
      list<double>::reverse_iterator last_parameter =
          (static_cast<CurvaParametrica *>(curve))->parametros.rend();
      --last_parameter;

      int parameter =
          static_cast<CurvaParametrica *>(curve)->getNumDePontos() - 1;

      for (list<double>::reverse_iterator param_iterator =
               (static_cast<CurvaParametrica *>(curve))->parametros.rbegin();
           param_iterator != last_parameter; ++param_iterator) {
        Vertex *vertex;

        if (i == 2)
          vertex = avanco.getBoundary()->addVertex(
              *param_iterator, 1.0, static_cast<CurvaParametrica *>(curve));
        else if (i == 3)
          vertex = avanco.getBoundary()->addVertex(
              0.0, *param_iterator, static_cast<CurvaParametrica *>(curve));

        Noh *noh = static_cast<Noh *>(curve->getPonto(parameter));

        map[vertex] = noh;

        sub_mesh_new->insereNoh(noh);

        parameter--;
      }
    }
  }

  avanco.getBoundary()->close(static_cast<CurvaParametrica *>(
      coons_patch->getCurva(coons_patch->getNumDeCurvas() - 1)));

  SubMalha *sub_mesh_old = coons_patch->getMalha();  // essa é a malha anterior!

  // constroi a lista de triangulos antigos para o gerador de malha
  FaceList mesh_old;

  double area_total = 0;  // área de todos os elementos

  for (unsigned int i = 0; i < sub_mesh_old->getNumDeElementos(); ++i) {
    Triangulo *tri = static_cast<Triangulo *>(sub_mesh_old->getElemento(i));

    /*Noh centro (	( tri->getN ( 1 ).x + tri->getN ( 2 ).x + tri->getN ( 3
).x ) / 3.0 , ( tri->getN ( 1 ).y + tri->getN ( 2 ).y + tri->getN ( 3 ).y  )
/ 3.0 , ( tri->getN ( 1 ).z + tri->getN ( 2 ).z + tri->getN ( 3 ).z  ) / 3.0
); tuple < double, double > centro_par = patch->encontrar_u_v ( centro );*/

    Vertex *v1 = new Vertex(get<0>(tri->p1), get<1>(tri->p1));
    Vertex *v2 = new Vertex(get<0>(tri->p2), get<1>(tri->p2));
    Vertex *v3 = new Vertex(get<0>(tri->p3), get<1>(tri->p3));
    // Vertex* c = new Vertex  ( get<0>( centro_par ), get<1>( centro_par ) );

    // cout << "APC.cpp: u = " << get<0>( centro_par ) << " v = " << get<1>(
    // centro_par ) << endl;

    Face *face = new Face(v1, v2, v3, tri->getId() /*, c*/);
    area_total += tri->getArea();

    mesh_old.push_back(face);
  }

  FaceList::iterator face_list_iterator = mesh_old.begin();

  for (unsigned int i = 0; i < sub_mesh_old->getNumDeElementos(); ++i) {
    double length_old = 0;

    Triangulo *tri = static_cast<Triangulo *>(sub_mesh_old->getElemento(i));
    Face *face = (*face_list_iterator);

    length_old = sqrt(tri->getArea() / area_total);

    double lenght_new = 0.0;
    double ka = 0.0;
    double kd = 0.0;

    if (fabs(tri->getN(1).Ga) >= TOLERANCIA and
        fabs(tri->getN(2).Ga) >= TOLERANCIA and
        fabs(tri->getN(3).Ga) >= TOLERANCIA) {
      ka = (tri->getN(1).Ga + tri->getN(2).Ga + tri->getN(3).Ga) / 3.0;
      kd = (tri->getN(1).Gd + tri->getN(2).Gd + tri->getN(3).Gd) / 3.0;
    } else {
      ka = (tri->getN(1).Ha + tri->getN(2).Ha + tri->getN(3).Ha) / 3.0;
      kd = (tri->getN(1).Hd + tri->getN(2).Hd + tri->getN(3).Hd) / 3.0;
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

    Noh *noh =
        new Noh(coons_patch->parametrizar(vertex->getX(), vertex->getY()));

    noh->id = id_manager->next(0);

    map[vertex] = noh;
    sub_mesh_new->insereNoh(noh);
  }

  // id_ele = 1;

  // gera os triangulos
  while (!new_mesh.empty()) {
    Face *face = new_mesh.front();
    new_mesh.pop_front();

    Elemento *element = new Triangulo(static_cast<Noh *>(map[face->getV1()]),
                                      static_cast<Noh *>(map[face->getV2()]),
                                      static_cast<Noh *>(map[face->getV3()]));

    element->setId(id_manager->next(1));

    (static_cast<Triangulo *>(element))->p1 =
        make_tuple(face->getV1()->getX(), face->getV1()->getY());
    (static_cast<Triangulo *>(element))->p2 =
        make_tuple(face->getV2()->getX(), face->getV2()->getY());
    (static_cast<Triangulo *>(element))->p3 =
        make_tuple(face->getV3()->getX(), face->getV3()->getY());

    sub_mesh_new->insereElemento(element);
  }

  // apaga os triangulos da malha antiga
  while (!mesh_old.empty()) {
    Face *face = mesh_old.front();
    mesh_old.pop_front();

    delete face;
  }

  return sub_mesh_new;
}

// #pragma omp parallel for num_threads(NUM_THREADS) firstprivate(atual,proxi)
//  shared(bt)
//     // 1.2. Para cada segmento da curva
//     for (int i = 0; i < pontos.size(); ++i)
//     {
//         // 1.2.1. Calcule o comprimento do segmento e guarde em h_velho
//         h_velho = c->calcularTamanho ( *(*atual), *(*proxi) );

//        // cout << "tamanho do segmento de curva"<<h_velho<< endl;

//        //cout << "calculou novo tamanho" << endl;

//        // 1.2.2. Calcule o ponto médio do segmento
//        C_seg = static_cast < CurvaParametrica* > ( c )->pontoMedio (
//        *(*atual), *(*proxi) );

//        // 1.2.2.1 Encontre o parâmetro do ponto médio
//        t = static_cast < CurvaParametrica* > ( c )->encontrar_t ( C_seg );

//        // 1.2.3. Calcule as curvaturas analítica e discreta do ponto médio

//        ka = c->calcularCurvatura(t);

//        kd = (c->calcularCurvatura(0) + c->calcularCurvatura(1)) / 2.0;

//        // 1.2.4. O novo tamanho é calculado de acordo com os cenários
//        h_novo = novoTamanho ( ka, kd, f, h_velho );

//        //cout << "calculou novo tamanho" << endl;

//        // 1.2.5. Calcule o tamanho paramétrico
//        h_par = h_novo / c->get_L ( );

//        bt.subdividir( t, h_par*fator_dis, (CurvaParametrica*)c );

//        ++proxi;
//        ++atual;
//    }
