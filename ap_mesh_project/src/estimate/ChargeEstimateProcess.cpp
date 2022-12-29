#include "../../include/estimate/ChargeEstimateProcess.h"

ChargeEstimateProcess::ChargeEstimateProcess() {
  menor_erro = 10000;
  menor_grau = 0;
}

static bool sortByNt(const PatchBezier* lhs, const PatchBezier* rhs) {
  return lhs->getKaMedio() > rhs->getKaMedio();
}

// retonar uma lista de patch de bezier ordenados de acordo com sua estimativa
// de carga em ordem decrescente.
std::list<PatchBezier*> ChargeEstimateProcess::chargeEstimateProcess(
    Geometry* geo, Timer* timer, std::string INPUT_MODEL) {
  std::list<PatchBezier*> listBezierPt;
  std::list<PatchBezier*> listBezierPtOrder;

  PatchBezierReader* pt = new PatchBezierReader();
  timer->endTimerParallel(0, 0, 10);  // Full
  timer->initTimerParallel(0, 0, 5);  // Leitura arquivo

  listBezierPt = pt->loaderBPFile(INPUT_MODEL);

  timer->endTimerParallel(0, 0, 5);  // Leitura arquivo
  TIME_READ_FILE = timer->timerParallel[0][0][5];

  timer->initTimerParallel(0, 0, 5);  // Full

  timer->initTimerParallel(0, 0, 1);  // Estimativa de carga process 0

  delete pt;

  Ponto* p00;
  Ponto* p01;
  Ponto* p02;
  Ponto* p03;
  Ponto* p10;
  Ponto* p11;
  Ponto* p12;
  Ponto* p13;
  Ponto* p20;
  Ponto* p21;
  Ponto* p22;
  Ponto* p23;
  Ponto* p30;
  Ponto* p31;
  Ponto* p32;
  Ponto* p33;

  CurveAdaptive* patch_c1;
  CurveAdaptive* patch_c2;
  CurveAdaptive* patch_c3;
  CurveAdaptive* patch_c4;

  double areaMenor = 1000;
  double areaMaior = 0;
  double kamMenor = 1000;
  double kamMaior = 0;

  double elementos = 0;

  // timer->endTimerParallel(0, 0, 8); // Over
  // timer->initTimerParallel(0, 0, 1); // Estimativa de carga process 0

  //     cout<<"INIT >> ESTIMATIVE"<< endl;

  for (std::list<PatchBezier*>::iterator it = listBezierPt.begin();
       it != listBezierPt.end(); it++) {
    p00 =
        new Vertice((*it)->GetPt00().x, (*it)->GetPt00().y, (*it)->GetPt00().z);
    p10 =
        new Vertice((*it)->GetPt10().x, (*it)->GetPt10().y, (*it)->GetPt10().z);
    p20 =
        new Vertice((*it)->GetPt20().x, (*it)->GetPt20().y, (*it)->GetPt20().z);
    p30 =
        new Vertice((*it)->GetPt30().x, (*it)->GetPt30().y, (*it)->GetPt30().z);

    p01 =
        new Vertice((*it)->GetPt01().x, (*it)->GetPt01().y, (*it)->GetPt01().z);
    p11 =
        new Vertice((*it)->GetPt11().x, (*it)->GetPt11().y, (*it)->GetPt11().z);
    p21 =
        new Vertice((*it)->GetPt21().x, (*it)->GetPt21().y, (*it)->GetPt21().z);
    p31 =
        new Vertice((*it)->GetPt31().x, (*it)->GetPt31().y, (*it)->GetPt31().z);

    p02 =
        new Vertice((*it)->GetPt02().x, (*it)->GetPt02().y, (*it)->GetPt02().z);
    p12 =
        new Vertice((*it)->GetPt12().x, (*it)->GetPt12().y, (*it)->GetPt12().z);
    p22 =
        new Vertice((*it)->GetPt22().x, (*it)->GetPt22().y, (*it)->GetPt22().z);
    p32 =
        new Vertice((*it)->GetPt32().x, (*it)->GetPt32().y, (*it)->GetPt32().z);

    p03 =
        new Vertice((*it)->GetPt03().x, (*it)->GetPt03().y, (*it)->GetPt03().z);
    p13 =
        new Vertice((*it)->GetPt13().x, (*it)->GetPt13().y, (*it)->GetPt13().z);
    p23 =
        new Vertice((*it)->GetPt23().x, (*it)->GetPt23().y, (*it)->GetPt23().z);
    p33 =
        new Vertice((*it)->GetPt33().x, (*it)->GetPt33().y, (*it)->GetPt33().z);

    if (geo->verifyCurveGeometria(p00, p10, p20, p30) == NULL) {
      patch_c1 = new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
      geo->insereCurva(patch_c1);
    } else {
      patch_c1 = geo->verifyCurveGeometria(p00, p10, p20, p30);
    }

    if (geo->verifyCurveGeometria(p30, p31, p32, p33) == NULL) {
      patch_c2 = new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
      geo->insereCurva(patch_c2);
    } else {
      patch_c2 = geo->verifyCurveGeometria(p30, p31, p32, p33);
    }

    if (geo->verifyCurveGeometria(p03, p13, p23, p33) == NULL) {
      patch_c3 = new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
      geo->insereCurva(patch_c3);
    } else {
      patch_c3 = geo->verifyCurveGeometria(p03, p13, p23, p33);
    }

    if (geo->verifyCurveGeometria(p00, p01, p02, p03) == NULL) {
      patch_c4 = new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);
      geo->insereCurva(patch_c4);
    } else {
      patch_c4 = geo->verifyCurveGeometria(p00, p01, p02, p03);
    }

    (*it) = new PatchBezier(patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21,
                            *p12, *p22);

    (*it)->setArea(calculateAreaPatch((*it), 4));
    // cout << "Área do patch: " << (*it)->getArea() << endl;

    if ((*it)->getArea() < areaMenor) {
      areaMenor = (*it)->getArea();
    } else if ((*it)->getArea() > areaMaior) {
      areaMaior = (*it)->getArea();
    }

    //(*it)->setKaMedio(calculateKaMedioPatch((*it), 4));
    // cout << "KAM: " << (*it)->getKaMedio() << endl;

    //        (*it)->setKaMedio(calculateKaMedioPatch((*it), 9));
    //        cout << "KAM: " << (*it)->getKaMedio() << endl;

    //    (*it)->setKaMedio(calculateKaMedioPatch((*it), 17));
    //    cout << "KAM: " << (*it)->getKaMedio() << endl;
    //
    (*it)->setKaMedio(calculateKaMedioPatch((*it), 25));
    //    cout << "KAM: " << (*it)->getKaMedio() << endl;

    if ((*it)->getKaMedio() < kamMenor) {
      kamMenor = (*it)->getKaMedio();
    } else if ((*it)->getKaMedio() > kamMaior) {
      kamMaior = (*it)->getKaMedio();
    }

    (*it)->setAreaTriangle(calculateAreaTriangleMedioRad((*it)));
    TRIANGLE_MEDIO = (*it)->getAreaTriangle();
    // cout << "Triângulo Medio 1: " << (*it)->getAreaTriangle() << endl;

    (*it)->setNumberTriangle((*it)->getArea() / (*it)->getAreaTriangle());
    // cout << "Quant. de triângulos do patch: " << (*it)->getNumberTriangle()
    // << endl;
    elementos += (*it)->getNumberTriangle();
    //        (*it)->setAreaTriangle(calculateAreaTriangleMedio((*it),4));
    //        cout << "Triângulo Medio 2: " << (*it)->getAreaTriangle() << endl;
    //
    //        (*it)->setNumberTriangle((*it)->getArea() /
    //        (*it)->getAreaTriangle()); cout << "Quant. de triângulos do patch:
    //        " << (*it)->getNumberTriangle() << endl;

    geo->inserePatch((*it));

    listBezierPtOrder.push_back((*it));

    vecCurvature.push_back((*it)->getKaMedio());
  }

  //    cout<<"INIT >> ANALISE CURVATURE"<< endl;
  if (WRITE_MESH == std::string("m")) {
    write.writeCurvaturePatches(vecCurvature, kamMaior);
  }
  //    cout<<"area menor: "<<areaMenor<<endl;
  //    cout<<"area maior: "<<areaMaior<<endl;
  //    cout<<"kam menor: "<<kamMenor<<endl;
  //    cout<<"kam maior: "<<kamMaior<<endl;
  //    cout<<"#elementos_estimados: "<<elementos<<endl;

  if (listBezierPtOrder.size() > 1) {
    listBezierPtOrder.sort(sortByNt);
  }
  //    for (std::list<BezierPatch*>::iterator it = listBezierPtOrder.begin();
  //    it != listBezierPtOrder.end(); it++) {
  //	// cout << "ordenado: " << (*it)->getNumberTriangle() << " area: " <<
  //(*it)->getArea()
  //	//     << " kam: " << (*it)->getKaMedio() << endl;
  //	// cout<<(*it)->getNumberTriangle()<<endl;
  //    }

  timer->endTimerParallel(0, 0, 1);  // Estimativa de carga process 0

  return listBezierPtOrder;
}

std::vector<Ponto> ChargeEstimateProcess::interpolateControlPointsCurve(
    Ponto p0, Ponto p1, Ponto p2, Ponto p3, double u, double v) {
  double a = 0.0, b = 0.0, c = 0.0, d = 0.0, det = 0.0;

  Ponto q1;
  Ponto q2;
  Ponto pc1;
  Ponto pc2;

  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0) || (u >= v)) {
    std::cout << "erro u or v out of range" << std::endl;
  }

  a = 3 * (1 - u) * (1 - u) * u;
  b = 3 * (1 - u) * u * u;
  c = 3 * (1 - v) * (1 - v) * v;
  d = 3 * (1 - v) * v * v;
  det = a * d - b * c;

  /* unnecessary, but just in case... */
  if (det == 0.0) {
    std::cout << "erro det == 0" << std::endl;
  }

  q1.x = (p1.x - ((1 - u) * (1 - u) * (1 - u) * p0.x + u * u * u * p3.x));
  q1.y = (p1.y - ((1 - u) * (1 - u) * (1 - u) * p0.y + u * u * u * p3.y));
  q1.z = (p1.z - ((1 - u) * (1 - u) * (1 - u) * p0.z + u * u * u * p3.z));

  q2.x = (p2.x - ((1 - v) * (1 - v) * (1 - v) * p0.x + v * v * v * p3.x));
  q2.y = (p2.y - ((1 - v) * (1 - v) * (1 - v) * p0.y + v * v * v * p3.y));
  q2.z = (p2.z - ((1 - v) * (1 - v) * (1 - v) * p0.z + v * v * v * p3.z));

  pc1.x = (d * q1.x - b * q2.x);
  pc1.y = (d * q1.y - b * q2.y);
  pc1.z = (d * q1.z - b * q2.z);

  pc1.x = (pc1.x / det);
  pc1.y = (pc1.y / det);
  pc1.z = (pc1.z / det);

  pc2.x = ((-c) * q1.x + a * q2.x);
  pc2.y = ((-c) * q1.y + a * q2.y);
  pc2.z = ((-c) * q1.z + a * q2.z);

  pc2.x = (pc2.x / det);
  pc2.y = (pc2.y / det);
  pc2.z = (pc2.z / det);

  std::vector<Ponto> list_pcs;

  list_pcs.push_back(pc1);
  list_pcs.push_back(pc2);

  return list_pcs;
}

double ChargeEstimateProcess::calculateKaMedioPatch(PatchBezier* patch,
                                                    int points) {
  if (points < 4) {
    cout << "Numero Mínimo de Pontos para o cálculo da curvatara média é 4 ou "
            "9 ou 17 ou 25"
         << endl;
    return 0.0;
  }

  double arrayKAM[points];
  int i = 0;

  if (points <= 25) {
    if (points >= 4) {
      Ponto* ponto_a = new Ponto();
      Ponto ponto_aa = patch->Parameterize(0.25, 0.25);
      ponto_a = &ponto_aa;

      Ponto* ponto_b = new Ponto();
      Ponto ponto_bb = patch->Parameterize(0.75, 0.25);
      ponto_b = &ponto_bb;

      Ponto* ponto_c = new Ponto();
      Ponto ponto_cc = patch->Parameterize(0.25, 0.75);
      ponto_c = &ponto_cc;

      Ponto* ponto_d = new Ponto();
      Ponto ponto_dd = patch->Parameterize(0.75, 0.75);
      ponto_d = &ponto_dd;

      CurvatureAnalytical kaa(*(static_cast<Noh*>(ponto_a)),
                              *(static_cast<PatchCoons*>(patch)));
      CurvatureAnalytical kab(*(static_cast<Noh*>(ponto_b)),
                              *(static_cast<PatchCoons*>(patch)));
      CurvatureAnalytical kac(*(static_cast<Noh*>(ponto_c)),
                              *(static_cast<PatchCoons*>(patch)));
      CurvatureAnalytical kad(*(static_cast<Noh*>(ponto_d)),
                              *(static_cast<PatchCoons*>(patch)));

      double ka_ponto_a = kaa.CalculateGaussCurvature();
      // if (fabs(ka_ponto_a) < 0, 0001) {
      if (fabs(ka_ponto_a) < 0.0001) {
        ka_ponto_a = kaa.CalculateMeanCurvature();
      }

      arrayKAM[i] = fabs(ka_ponto_a);
      i++;

      double kb_ponto_b = kab.CalculateGaussCurvature();
      // if (fabs(kb_ponto_b) < 0, 0001) {
      if (fabs(kb_ponto_b) < 0.0001) {
        kb_ponto_b = kab.CalculateMeanCurvature();
      }

      arrayKAM[i] = fabs(kb_ponto_b);
      i++;

      double kc_ponto_c = kac.CalculateGaussCurvature();
      // if (fabs(kc_ponto_c) < 0, 0001) {
      if (fabs(kc_ponto_c) < 0.0001) {
        kc_ponto_c = kac.CalculateMeanCurvature();
      }

      arrayKAM[i] = fabs(kc_ponto_c);
      i++;

      double kd_ponto_d = kad.CalculateGaussCurvature();
      // if (fabs(kd_ponto_d) < 0, 0001) {
      if (fabs(kd_ponto_d) < 0.0001) {
        kd_ponto_d = kad.CalculateMeanCurvature();
      }

      arrayKAM[i] = fabs(kd_ponto_d);
      i++;

      if (points >= 9) {
        Ponto* ponto_e = new Ponto();
        Ponto ponto_ee = patch->Parameterize(0.5, 0.5);
        ponto_e = &ponto_ee;

        Ponto* ponto_f = new Ponto();
        Ponto ponto_ff = patch->Parameterize(0.0, 0.0);
        ponto_f = &ponto_ff;

        Ponto* ponto_g = new Ponto();
        Ponto ponto_gg = patch->Parameterize(1.0, 0.0);
        ponto_g = &ponto_gg;

        Ponto* ponto_h = new Ponto();
        Ponto ponto_hh = patch->Parameterize(0.0, 1.0);
        ponto_h = &ponto_hh;

        Ponto* ponto_i = new Ponto();
        Ponto ponto_ii = patch->Parameterize(1.0, 1.0);
        ponto_i = &ponto_ii;

        CurvatureAnalytical kae(*(static_cast<Noh*>(ponto_e)),
                                *(static_cast<PatchCoons*>(patch)));
        CurvatureAnalytical kaf(*(static_cast<Noh*>(ponto_f)),
                                *(static_cast<PatchCoons*>(patch)));
        CurvatureAnalytical kag(*(static_cast<Noh*>(ponto_g)),
                                *(static_cast<PatchCoons*>(patch)));
        CurvatureAnalytical kah(*(static_cast<Noh*>(ponto_h)),
                                *(static_cast<PatchCoons*>(patch)));
        CurvatureAnalytical kai(*(static_cast<Noh*>(ponto_i)),
                                *(static_cast<PatchCoons*>(patch)));

        double ke_ponto_e = kae.CalculateGaussCurvature();
        // if (fabs(ke_ponto_e) < 0, 0001) {
        if (fabs(ke_ponto_e) < 0.0001) {
          ke_ponto_e = kae.CalculateMeanCurvature();
        }

        arrayKAM[i] = fabs(ke_ponto_e);
        i++;

        double kf_ponto_f = kaf.CalculateGaussCurvature();
        // if (fabs(kf_ponto_f) < 0, 0001) {
        if (fabs(kf_ponto_f) < 0.0001) {
          kf_ponto_f = kaf.CalculateMeanCurvature();
        }

        arrayKAM[i] = fabs(kf_ponto_f);
        i++;

        double kg_ponto_g = kag.CalculateGaussCurvature();
        // if (fabs(kg_ponto_g) < 0, 0001) {
        if (fabs(kg_ponto_g) < 0.0001) {
          kg_ponto_g = kag.CalculateMeanCurvature();
        }

        arrayKAM[i] = fabs(kg_ponto_g);
        i++;

        double kh_ponto_h = kah.CalculateGaussCurvature();
        // if (fabs(kh_ponto_h) < 0, 0001) {
        if (fabs(kh_ponto_h) < 0.0001) {
          kh_ponto_h = kah.CalculateMeanCurvature();
        }

        arrayKAM[i] = fabs(kh_ponto_h);
        i++;

        double ki_ponto_i = kai.CalculateGaussCurvature();
        // if (fabs(ki_ponto_i) < 0, 0001) {
        if (fabs(ki_ponto_i) < 0.0001) {
          ki_ponto_i = kai.CalculateMeanCurvature();
        }

        arrayKAM[i] = fabs(ki_ponto_i);
        i++;

        if (points >= 17) {
          Ponto* ponto_j = new Ponto();
          Ponto ponto_jj = patch->Parameterize(0.125, 0.125);
          ponto_j = &ponto_jj;

          Ponto* ponto_l = new Ponto();
          Ponto ponto_ll = patch->Parameterize(0.5, 0.125);
          ponto_l = &ponto_ll;

          Ponto* ponto_m = new Ponto();
          Ponto ponto_mm = patch->Parameterize(0.875, 0.125);
          ponto_m = &ponto_mm;

          Ponto* ponto_n = new Ponto();
          Ponto ponto_nn = patch->Parameterize(0.125, 0.5);
          ponto_n = &ponto_nn;

          Ponto* ponto_o = new Ponto();
          Ponto ponto_oo = patch->Parameterize(0.875, 0.5);
          ponto_o = &ponto_oo;

          Ponto* ponto_p = new Ponto();
          Ponto ponto_pp = patch->Parameterize(0.125, 0.875);
          ponto_p = &ponto_pp;

          Ponto* ponto_q = new Ponto();
          Ponto ponto_qq = patch->Parameterize(0.5, 0.875);
          ponto_q = &ponto_qq;

          Ponto* ponto_r = new Ponto();
          Ponto ponto_rr = patch->Parameterize(0.875, 0.875);
          ponto_r = &ponto_rr;

          CurvatureAnalytical kaj(*(static_cast<Noh*>(ponto_j)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kal(*(static_cast<Noh*>(ponto_l)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kam(*(static_cast<Noh*>(ponto_m)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kan(*(static_cast<Noh*>(ponto_n)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kao(*(static_cast<Noh*>(ponto_o)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kap(*(static_cast<Noh*>(ponto_p)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kaq(*(static_cast<Noh*>(ponto_q)),
                                  *(static_cast<PatchCoons*>(patch)));
          CurvatureAnalytical kar(*(static_cast<Noh*>(ponto_r)),
                                  *(static_cast<PatchCoons*>(patch)));

          double kj_ponto_j = kaj.CalculateGaussCurvature();
          // if (fabs(kj_ponto_j) < 0, 0001) {
          if (fabs(kj_ponto_j) < 0.0001) {
            kj_ponto_j = kaj.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kj_ponto_j);
          i++;

          double kl_ponto_l = kal.CalculateGaussCurvature();
          // if (fabs(kl_ponto_l) < 0, 0001) {
          if (fabs(kl_ponto_l) < 0.0001) {
            kl_ponto_l = kal.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kl_ponto_l);
          i++;

          double km_ponto_m = kam.CalculateGaussCurvature();
          // if (fabs(km_ponto_m) < 0, 0001) {
          if (fabs(km_ponto_m) < 0.0001) {
            km_ponto_m = kam.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(km_ponto_m);
          i++;

          double kn_ponto_n = kan.CalculateGaussCurvature();
          // if (fabs(kn_ponto_n) < 0, 0001) {
          if (fabs(kn_ponto_n) < 0.0001) {
            kn_ponto_n = kan.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kn_ponto_n);
          i++;

          double ko_ponto_o = kao.CalculateGaussCurvature();
          // if (fabs(ko_ponto_o) < 0, 0001) {
          if (fabs(ko_ponto_o) < 0.0001) {
            ko_ponto_o = kao.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(ko_ponto_o);
          i++;

          double kp_ponto_p = kap.CalculateGaussCurvature();
          // if (fabs(kp_ponto_p) < 0, 0001) {
          if (fabs(kp_ponto_p) < 0.0001) {
            kp_ponto_p = kap.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kp_ponto_p);
          i++;

          double kq_ponto_q = kaq.CalculateGaussCurvature();
          // if (fabs(kq_ponto_q) < 0, 0001) {
          if (fabs(kq_ponto_q) < 0.0001) {
            kq_ponto_q = kaq.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kq_ponto_q);
          i++;

          double kr_ponto_r = kar.CalculateGaussCurvature();
          // if (fabs(kr_ponto_r) < 0, 0001) {
          if (fabs(kr_ponto_r) < 0.0001) {
            kr_ponto_r = kar.CalculateMeanCurvature();
          }

          arrayKAM[i] = fabs(kr_ponto_r);
          i++;

          if (points == 25) {
            Ponto* ponto_s = new Ponto();
            Ponto ponto_ss = patch->Parameterize(0.25, 0.0);
            ponto_s = &ponto_ss;

            Ponto* ponto_t = new Ponto();
            Ponto ponto_tt = patch->Parameterize(0.75, 0.0);
            ponto_t = &ponto_tt;

            Ponto* ponto_u = new Ponto();
            Ponto ponto_uu = patch->Parameterize(0.0, 0.25);
            ponto_u = &ponto_uu;

            Ponto* ponto_v = new Ponto();
            Ponto ponto_vv = patch->Parameterize(1.0, 0.25);
            ponto_v = &ponto_vv;

            Ponto* ponto_w = new Ponto();
            Ponto ponto_ww = patch->Parameterize(0.0, 0.75);
            ponto_w = &ponto_ww;

            Ponto* ponto_x = new Ponto();
            Ponto ponto_xx = patch->Parameterize(1.0, 0.75);
            ponto_x = &ponto_xx;

            Ponto* ponto_y = new Ponto();
            Ponto ponto_yy = patch->Parameterize(0.25, 1.0);
            ponto_y = &ponto_yy;

            Ponto* ponto_z = new Ponto();
            Ponto ponto_zz = patch->Parameterize(0.75, 1.0);
            ponto_z = &ponto_zz;

            CurvatureAnalytical kas(*(static_cast<Noh*>(ponto_s)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kat(*(static_cast<Noh*>(ponto_t)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kau(*(static_cast<Noh*>(ponto_u)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kav(*(static_cast<Noh*>(ponto_v)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kaw(*(static_cast<Noh*>(ponto_w)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kax(*(static_cast<Noh*>(ponto_x)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kay(*(static_cast<Noh*>(ponto_y)),
                                    *(static_cast<PatchCoons*>(patch)));
            CurvatureAnalytical kaz(*(static_cast<Noh*>(ponto_z)),
                                    *(static_cast<PatchCoons*>(patch)));

            double ks_ponto_s = kas.CalculateGaussCurvature();
            // if (fabs(ks_ponto_s) < 0, 0001) {
            if (fabs(ks_ponto_s) < 0.0001) {
              ks_ponto_s = kas.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(ks_ponto_s);
            i++;

            double kt_ponto_t = kat.CalculateGaussCurvature();
            // if (fabs(kt_ponto_t) < 0, 0001) {
            if (fabs(kt_ponto_t) < 0.0001) {
              kt_ponto_t = kat.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(kt_ponto_t);
            i++;

            double ku_ponto_u = kau.CalculateGaussCurvature();
            // if (fabs(ku_ponto_u) < 0, 0001) {
            if (fabs(ku_ponto_u) < 0.0001) {
              ku_ponto_u = kau.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(ku_ponto_u);
            i++;

            double kv_ponto_v = kav.CalculateGaussCurvature();
            // if (fabs(kv_ponto_v) < 0, 0001) {
            if (fabs(kv_ponto_v) < 0.0001) {
              kv_ponto_v = kav.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(kv_ponto_v);
            i++;

            double kw_ponto_w = kaw.CalculateGaussCurvature();
            // if (fabs(kw_ponto_w) < 0, 0001) {
            if (fabs(kw_ponto_w) < 0.0001) {
              kw_ponto_w = kaw.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(kw_ponto_w);
            i++;

            double kx_ponto_x = kax.CalculateGaussCurvature();
            // if (fabs(kx_ponto_x) < 0, 0001) {
            if (fabs(kx_ponto_x) < 0.0001) {
              kx_ponto_x = kax.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(kx_ponto_x);
            i++;

            double ky_ponto_y = kay.CalculateGaussCurvature();
            // if (fabs(ky_ponto_y) < 0, 0001) {
            if (fabs(ky_ponto_y) < 0.0001) {
              ky_ponto_y = kay.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(ky_ponto_y);
            i++;

            double kz_ponto_z = kaz.CalculateGaussCurvature();
            // if (fabs(kz_ponto_z) < 0, 0001) {
            if (fabs(kz_ponto_z) < 0.0001) {
              kz_ponto_z = kaz.CalculateMeanCurvature();
            }

            arrayKAM[i] = fabs(kz_ponto_z);
            i++;
          }
        }
      }
    }
  }

  double result = 0.0;
  for (int j = 0; j < i; j++) {
    // cout<<arrayKAM[j]<<endl;
    result += arrayKAM[j];
  }

  return result / points;
}

double ChargeEstimateProcess::calculateAreaPatch(PatchBezier* patch,
                                                 int pointesGauusLegandre) {
  Vetor V;
  double I = 0.0;
  double beta[pointesGauusLegandre];
  double peso[pointesGauusLegandre];
  double u = 0.0, v = 0.0;

  switch (pointesGauusLegandre) {
    case 3: {
      // Gauss–Legendre, valores para 3 pontos
      beta[0] = -sqrt(3.0 / 5.0);
      beta[1] = 0.0;
      beta[2] = sqrt(3.0 / 5.0);
      peso[0] = 5.0 / 9.0;
      peso[1] = 8.0 / 9.0;
      peso[2] = 5.0 / 9.0;
      //   mult = 0.25;
      break;
    }

    case 4: {
      // Gauss–Legendre, valores para 4 pontos
      beta[0] = -0.861136;
      beta[1] = -0.339981;
      beta[2] = 0.339981;
      beta[3] = 0.861136;
      peso[0] = 0.347855;
      peso[1] = peso[2] = 0.652145;
      peso[3] = 0.347855;
      //  mult = 0.25;
      break;
    }
    case 5: {
      // Gauss–Legendre, valores para 5 pontos
      //  beta[0] = -sqrt(3.0/5.0); beta[1] = 0.0; beta[2] = sqrt(3.0/5.0);
      //  peso[0] = 5.0/9.0; peso[1] = 8.0/9.0; peso[2] = 5.0/9.0;
      //  mult = 0.;

      break;
    }
  }

  for (int i = 0; i < pointesGauusLegandre; ++i) {
    u = 0.5 * (1 + beta[i]);

    for (int j = 0; j < pointesGauusLegandre; ++j) {
      v = 0.5 * (1 + beta[j]);

      Vetor Pu = patch->Qu(u, v);
      Vetor Pv = patch->Qv(u, v);

      V.x = Pu.y * Pv.z - Pu.z * Pv.y;
      V.y = Pu.z * Pv.x - Pu.x * Pv.z;
      V.z = Pu.x * Pv.y - Pu.y * Pv.x;

      I = I + 0.25 * ((sqrt(pow(V.x, 2) + pow(V.y, 2) + pow(V.z, 2))) *
                      peso[i] * peso[j]);
    }
  }

  return I;
}

double ChargeEstimateProcess::calculateAreaTriangleMedioRad(
    PatchBezier* patch) {
  double Kam = patch->getKaMedio();

  // cout << "Kam " << Kam << endl;

  double raio = 1.0 / Kam;
  // cout << "Raio " << raio << endl;

  double rad = (1.0 / 20) * raio;

  // cout << "Rad " << rad << endl;

  // double lado = (rad * 180) / M_PI;

  // cout << "Lado " << lado << endl;

  double area = pow(rad, 2) * sqrt(3) / 4;

  // cout << "Area " << area << endl;

  return area;
}

double ChargeEstimateProcess::calculateAreaTriangleMedio(PatchBezier* patch,
                                                         Timer* timer,
                                                         int grau) {
  MeshAdaptive* malha = new MeshAdaptive;
  SubMesh* sub = malhaInicialEstimativa(patch, grau);
  malha->InsertSubMeshAdaptive(sub);
  // delete sub;

  while (calculateErroEstimative(malha, timer, grau) && grau < 5) {
    ++grau;
    //        cout<<"grau: "<<grau<<endl;
    //        cout<<"sub: "<<sub<<endl;
    SubMesh* sub1 = malhaInicialEstimativa(patch, grau);
    malha->RemoveSubMeshAdaptive();
    malha->InsertSubMeshAdaptive(sub1);
    // delete sub1;
  }

  // cout<<"menor_grau: "<<menor_grau<<endl;

  SubMesh* sub2 = malhaInicialEstimativa(patch, menor_grau);
  malha->RemoveSubMeshAdaptive();
  malha->InsertSubMeshAdaptive(sub2);

  static_cast<PatchBezier*>(patch)->setAreaTriangle(
      malha->GetSubMeshAdaptiveByPosition(0)->GetElement(0)->GetArea());
  // delete malha;
  return patch->getAreaTriangle();
}

long ChargeEstimateProcess::calculateNumbersTriangle(PatchBezier* patch,
                                                     int grau) {
  SubMesh* sub = malhaInicialEstimativa(patch, grau);

  return sub->GetNumberElements();
}

// grau tem que ser multiplo de grau == 2^n
SubMesh* ChargeEstimateProcess::malhaInicialEstimativa(PatchCoons* patch,
                                                       int grau) {
  int idv = 1;
  int ide = 1;
  int salto = grau;
  int comprimento = grau + 1;
  int total_1 = 0;
  int total_2 = 0;

  CurveAdaptive* c1 = patch->GetCurve(0);
  CurveAdaptive* c2 = patch->GetCurve(1);
  CurveAdaptive* c3 = patch->GetCurve(2);
  CurveAdaptive* c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = NULL;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = NULL;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = NULL;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = NULL;  // c4 já foi trabalhada no patch vizinho

  SubMesh* sub = new SubMesh;

  for (double v = 0.0; v <= 1.0; v += 1.0 / grau) {
    for (double u = 0.0; u <= 1.0; u += 1.0 / grau) {
      Ponto* p = new Noh(patch->Parameterize(u, v));
      p->id = idv++;

      // cout << "u = " << u << " v = " << v << endl;
      if (v == 0 and c1)  // p está na curva 1
        c1->InsertPoint(p);
      else if (v == 1 and c3)  // p está na curva 3
        c3->InsertPoint(p);

      if (u == 0 and c4)  // p está na curva 4
        c4->InsertPoint(p);
      else if (u == 1 and c2)  // p está na curva 2
        c2->InsertPoint(p);

      sub->SetNoh(static_cast<Noh*>(p));
      total_1++;
    }
  }

  for (double v = 1.0 / (2.0 * grau); v <= 1.0 - (1.0 / (2.0 * grau));
       v += 1.0 / grau) {
    for (double u = 1.0 / (2.0 * grau); u <= 1.0 - (1.0 / (2.0 * grau));
         u += 1.0 / grau) {
      //   cout << "u = " << u << " v = " << v << endl;
      Ponto* p = new Noh(patch->Parameterize(u, v));
      sub->SetNoh(static_cast<Noh*>(p));
      p->id = idv++;
    }
  }

  total_2 = total_1;

  for (int i = 0; i < (total_2 - comprimento - 1); ++i) {
    if (i == salto) {
      salto = salto + comprimento;
      ++i;
      total_1 = total_1 - 1;
    }

    ElementAdaptive* e1 = new Triangulo(sub->GetNoh(i), sub->GetNoh(i + 1),
                                        sub->GetNoh(i + total_1));
    ((Triangulo*)e1)->p1 = patch->FindUV(*(sub->GetNoh(i)));
    ((Triangulo*)e1)->p2 = patch->FindUV(*(sub->GetNoh(i + 1)));
    ((Triangulo*)e1)->p3 = patch->FindUV(*(sub->GetNoh(i + total_1)));
    e1->SetId(ide++);
    sub->SetElement(e1);

    ElementAdaptive* e2 =
        new Triangulo(sub->GetNoh(i + 1), sub->GetNoh(i + comprimento + 1),
                      sub->GetNoh(i + total_1));
    ((Triangulo*)e2)->p1 = patch->FindUV(*(sub->GetNoh(i + 1)));
    ((Triangulo*)e2)->p2 = patch->FindUV(*(sub->GetNoh(i + comprimento + 1)));
    ((Triangulo*)e2)->p3 = patch->FindUV(*(sub->GetNoh(i + total_1)));
    e2->SetId(ide++);
    sub->SetElement(e2);

    ElementAdaptive* e3 =
        new Triangulo(sub->GetNoh(i + comprimento + 1),
                      sub->GetNoh(i + comprimento), sub->GetNoh(i + total_1));
    ((Triangulo*)e3)->p1 = patch->FindUV(*(sub->GetNoh(i + comprimento + 1)));
    ((Triangulo*)e3)->p2 = patch->FindUV(*(sub->GetNoh(i + comprimento)));
    ((Triangulo*)e3)->p3 = patch->FindUV(*(sub->GetNoh(i + total_1)));
    e3->SetId(ide++);
    sub->SetElement(e3);

    ElementAdaptive* e4 = new Triangulo(
        sub->GetNoh(i + comprimento), sub->GetNoh(i), sub->GetNoh(i + total_1));
    ((Triangulo*)e4)->p1 = patch->FindUV(*(sub->GetNoh(i + comprimento)));
    ((Triangulo*)e4)->p2 = patch->FindUV(*(sub->GetNoh(i)));
    ((Triangulo*)e4)->p3 = patch->FindUV(*(sub->GetNoh(i + total_1)));
    e4->SetId(ide++);
    sub->SetElement(e4);
    // cout<<"i "<<i<<endl;
  }

  // 5. define a submalha do patch
  patch->SetSubMesh(sub);
  sub->SetPatch(patch);

  MeshAdaptive* malha = new MeshAdaptive;
  malha->InsertSubMeshAdaptive(sub);

  return sub;
}

bool ChargeEstimateProcess::calculateErroEstimative(MeshAdaptive* malha,
                                                    Timer* timer, int grau) {
  GeradorAdaptativoPorCurvatura* ger = new GeradorAdaptativoPorCurvatura();
#if USE_OPENMP
  double erro = ger->erroGlobalOmp(malha, timer);
#else
  double erro = ger->erroGlobal(malha, timer);
#endif
  // delete ger;

  // cout << "erro: " << erro << " tolerancia: " << TOLERANCIA_ESTIMATIVE <<
  // endl;

  if (erro < menor_erro) {
    menor_grau = grau;
  }

  if (erro <= TOLERANCIA_ESTIMATIVE) {
    //  cout<<"return: false" <<endl;
    return false;
  }
  // cout<<"return: true" <<endl;

  return true;
}

ChargeEstimateProcess::~ChargeEstimateProcess() {}
