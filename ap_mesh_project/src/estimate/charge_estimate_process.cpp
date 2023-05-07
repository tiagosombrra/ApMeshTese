#include "../../include/estimate/charge_estimate_process.h"

#include "../../include/definitions.h"
#include "../../include/generator/generator_adaptive.h"

extern double timeReadFile, kEstimativeTolerance;
extern std::string inputModel, writeMesh;

ChargeEstimateProcess::ChargeEstimateProcess()
    : minor_error_(std::numeric_limits<double>::max()),
      minor_degree_(0),
      triangle_medio_(0) {}

ChargeEstimateProcess::~ChargeEstimateProcess() {}

static bool SortByNt(const std::shared_ptr<PatchBezier>& lhs,
                     const std::shared_ptr<PatchBezier>& rhs) {
  return lhs->GetKaMedio() > rhs->GetKaMedio();
}

// retonar uma lista de patch de bezier ordenadChargeEstimateProcessestimativa
// de carga em ordem decrescente.
std::list<std::shared_ptr<PatchBezier>> ChargeEstimateProcess::ChargeEstimate(
    std::shared_ptr<Geometry>& geometry, Timer& timer) {
  std::list<std::shared_ptr<PatchBezier>> list_patch_bezier;
  std::list<std::shared_ptr<PatchBezier>> list_patch_bezier_order;

  timer.EndTimerParallel(0, 0, 10);  // Full
  timer.InitTimerParallel(0, 0, 5);  // Leitura arquivo

  PatchReader patch_bezier_reader;
  list_patch_bezier = patch_bezier_reader.LoaderBPFile(inputModel);

  timer.EndTimerParallel(0, 0, 5);  // Leitura arquivo
  timeReadFile = timer.timer_parallel_[0][0][5];
  timer.InitTimerParallel(0, 0, 5);  // Full
  timer.InitTimerParallel(0, 0, 1);  // Estimativa de carga process 0

  double area_minor = 1000;
  double area_major = 0;
  double kam_minor = 1000;
  double kam_major = 0;

  [[maybe_unused]] double elementos = 0;

  std::shared_ptr<CurveAdaptiveParametric> patch_c1;
  std::shared_ptr<CurveAdaptiveParametric> patch_c2;
  std::shared_ptr<CurveAdaptiveParametric> patch_c3;
  std::shared_ptr<CurveAdaptiveParametric> patch_c4;

  for (auto& patch : list_patch_bezier) {
    auto p00 = std::make_shared<VertexAdaptive>((patch)->GetPt00().GetX(),
                                                (patch)->GetPt00().GetY(),
                                                (patch)->GetPt00().GetZ());
    auto p10 = std::make_shared<VertexAdaptive>((patch)->GetPt10().GetX(),
                                                (patch)->GetPt10().GetY(),
                                                (patch)->GetPt10().GetZ());
    auto p20 = std::make_shared<VertexAdaptive>((patch)->GetPt20().GetX(),
                                                (patch)->GetPt20().GetY(),
                                                (patch)->GetPt20().GetZ());
    auto p30 = std::make_shared<VertexAdaptive>((patch)->GetPt30().GetX(),
                                                (patch)->GetPt30().GetY(),
                                                (patch)->GetPt30().GetZ());

    auto p01 = std::make_shared<VertexAdaptive>((patch)->GetPt01().GetX(),
                                                (patch)->GetPt01().GetY(),
                                                (patch)->GetPt01().GetZ());
    auto p11 = std::make_shared<VertexAdaptive>((patch)->GetPt11().GetX(),
                                                (patch)->GetPt11().GetY(),
                                                (patch)->GetPt11().GetZ());
    auto p21 = std::make_shared<VertexAdaptive>((patch)->GetPt21().GetX(),
                                                (patch)->GetPt21().GetY(),
                                                (patch)->GetPt21().GetZ());
    auto p31 = std::make_shared<VertexAdaptive>((patch)->GetPt31().GetX(),
                                                (patch)->GetPt31().GetY(),
                                                (patch)->GetPt31().GetZ());

    auto p02 = std::make_shared<VertexAdaptive>((patch)->GetPt02().GetX(),
                                                (patch)->GetPt02().GetY(),
                                                (patch)->GetPt02().GetZ());
    auto p12 = std::make_shared<VertexAdaptive>((patch)->GetPt12().GetX(),
                                                (patch)->GetPt12().GetY(),
                                                (patch)->GetPt12().GetZ());
    auto p22 = std::make_shared<VertexAdaptive>((patch)->GetPt22().GetX(),
                                                (patch)->GetPt22().GetY(),
                                                (patch)->GetPt22().GetZ());
    auto p32 = std::make_shared<VertexAdaptive>((patch)->GetPt32().GetX(),
                                                (patch)->GetPt32().GetY(),
                                                (patch)->GetPt32().GetZ());

    auto p03 = std::make_shared<VertexAdaptive>((patch)->GetPt03().GetX(),
                                                (patch)->GetPt03().GetY(),
                                                (patch)->GetPt03().GetZ());
    auto p13 = std::make_shared<VertexAdaptive>((patch)->GetPt13().GetX(),
                                                (patch)->GetPt13().GetY(),
                                                (patch)->GetPt13().GetZ());
    auto p23 = std::make_shared<VertexAdaptive>((patch)->GetPt23().GetX(),
                                                (patch)->GetPt23().GetY(),
                                                (patch)->GetPt23().GetZ());
    auto p33 = std::make_shared<VertexAdaptive>((patch)->GetPt33().GetX(),
                                                (patch)->GetPt33().GetY(),
                                                (patch)->GetPt33().GetZ());

    if (geometry->VerifyCurveGeometry(*p00, *p10, *p20, *p30) == nullptr) {
      patch_c1 = std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p10,
                                                                 *p20, *p30);
      geometry->InsertCurve(patch_c1);
    } else {
      patch_c1 = geometry->VerifyCurveGeometry(*p00, *p10, *p20, *p30);
    }

    if (geometry->VerifyCurveGeometry(*p30, *p31, *p32, *p33) == nullptr) {
      patch_c2 = std::make_shared<CurveAdaptiveParametricBezier>(*p30, *p31,
                                                                 *p32, *p33);
      geometry->InsertCurve(patch_c2);
    } else {
      patch_c2 = geometry->VerifyCurveGeometry(*p30, *p31, *p32, *p33);
    }

    if (geometry->VerifyCurveGeometry(*p03, *p13, *p23, *p33) == nullptr) {
      patch_c3 = std::make_shared<CurveAdaptiveParametricBezier>(*p03, *p13,
                                                                 *p23, *p33);
      geometry->InsertCurve(patch_c3);
    } else {
      patch_c3 = geometry->VerifyCurveGeometry(*p03, *p13, *p23, *p33);
    }

    if (geometry->VerifyCurveGeometry(*p00, *p01, *p02, *p03) == nullptr) {
      patch_c4 = std::make_shared<CurveAdaptiveParametricBezier>(*p00, *p01,
                                                                 *p02, *p03);
      geometry->InsertCurve(patch_c4);
    } else {
      patch_c4 = geometry->VerifyCurveGeometry(*p00, *p01, *p02, *p03);
    }

    patch = std::make_shared<PatchBezier>(patch_c1, patch_c2, patch_c3,
                                          patch_c4, *p11, *p21, *p12, *p22);

    patch->SetArea(CalculateAreaPatch(patch, 4));
    // cout << "Área do patch: " << (patch)->getArea() << endl;

    if (patch->GetArea() < area_minor) {
      area_minor = patch->GetArea();
    } else if (patch->GetArea() > area_major) {
      area_major = patch->GetArea();
    }

    patch->SetKaMedio(CalculateKaMedioPatch(patch, 25));

    if (patch->GetKaMedio() < kam_minor) {
      kam_minor = patch->GetKaMedio();
    } else if (patch->GetKaMedio() > kam_major) {
      kam_major = patch->GetKaMedio();
    }

    patch->SetAreaTriangle(CalculateAreaTriangleMedioRad(patch));
    triangle_medio_ = patch->GetAreaTriangle();

    patch->SetNumberTriangle(patch->GetArea() / patch->GetAreaTriangle());

    elementos += patch->GetNumberTriangle();

    geometry->InsertPatch(patch);

    list_patch_bezier_order.push_back(patch);

    curvatures_.push_back(patch->GetKaMedio());
  }

  if (writeMesh == std::string("m")) {
    write_obj_file_.WriteCurvaturePatches(curvatures_, kam_major);
  }

  if (list_patch_bezier_order.size() > 1) {
    list_patch_bezier_order.sort(SortByNt);
  }

  timer.EndTimerParallel(0, 0, 1);  // Estimativa de carga process 0

  return list_patch_bezier_order;
}

std::vector<PointAdaptive> ChargeEstimateProcess::InterpolateControlPointsCurve(
    const PointAdaptive p0, const PointAdaptive p1, const PointAdaptive p2,
    const PointAdaptive p3, const double u, const double v) {
  double a = 0.0, b = 0.0, c = 0.0, d = 0.0, det = 0.0;

  PointAdaptive q1;
  PointAdaptive q2;
  PointAdaptive pc1;
  PointAdaptive pc2;

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

  q1.SetX((p1.GetX() -
           ((1 - u) * (1 - u) * (1 - u) * p0.GetX() + u * u * u * p3.GetX())));
  q1.SetY((p1.GetY() -
           ((1 - u) * (1 - u) * (1 - u) * p0.GetY() + u * u * u * p3.GetY())));
  q1.SetZ((p1.GetZ() -
           ((1 - u) * (1 - u) * (1 - u) * p0.GetZ() + u * u * u * p3.GetZ())));

  q2.SetX((p2.GetX() -
           ((1 - v) * (1 - v) * (1 - v) * p0.GetX() + v * v * v * p3.GetX())));
  q2.SetY((p2.GetY() -
           ((1 - v) * (1 - v) * (1 - v) * p0.GetY() + v * v * v * p3.GetY())));
  q2.SetZ((p2.GetZ() -
           ((1 - v) * (1 - v) * (1 - v) * p0.GetZ() + v * v * v * p3.GetZ())));

  pc1.SetX((d * q1.GetX() - b * q2.GetX()));
  pc1.SetY((d * q1.GetY() - b * q2.GetY()));
  pc1.SetZ((d * q1.GetZ() - b * q2.GetZ()));

  pc1.SetX((pc1.GetX() / det));
  pc1.SetY((pc1.GetY() / det));
  pc1.SetZ((pc1.GetZ() / det));

  pc2.SetX(((-c) * q1.GetX() + a * q2.GetX()));
  pc2.SetY(((-c) * q1.GetY() + a * q2.GetY()));
  pc2.SetZ(((-c) * q1.GetZ() + a * q2.GetZ()));

  pc2.SetX((pc2.GetX() / det));
  pc2.SetY((pc2.GetY() / det));
  pc2.SetZ((pc2.GetZ() / det));

  std::vector<PointAdaptive> list_pcs;

  list_pcs.push_back(pc1);
  list_pcs.push_back(pc2);

  return list_pcs;
}

double ChargeEstimateProcess::CalculateKaMedioPatch(
    std::shared_ptr<PatchBezier>& patch, int points) {
  if (points < 4) {
    cout << "Numero Mínimo de Pontos para o cálculo da curvatara média é 4 ou "
            "9 ou 17 ou 25"
         << endl;
    return 0.0;
  }

  double array_kam[points];
  int i = 0;

  if (points <= 25) {
    if (points >= 4) {
      PointAdaptive ponto_a = patch->Parameterize(0.25, 0.25);
      PointAdaptive ponto_b = patch->Parameterize(0.75, 0.25);
      PointAdaptive ponto_c = patch->Parameterize(0.25, 0.75);
      PointAdaptive ponto_d = patch->Parameterize(0.75, 0.75);

      CurvatureAnalytical kaa(static_cast<NodeAdaptive>(ponto_a),
                              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
      CurvatureAnalytical kab(static_cast<NodeAdaptive>(ponto_b),
                              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
      CurvatureAnalytical kac(static_cast<NodeAdaptive>(ponto_c),
                              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
      CurvatureAnalytical kad(static_cast<NodeAdaptive>(ponto_d),
                              *(std::dynamic_pointer_cast<PatchCoons>(patch)));

      double ka_ponto_a = kaa.CalculateGaussCurvature();
      // if (fabs(ka_ponto_a) < 0, 0001) {
      if (fabs(ka_ponto_a) < 0.0001) {
        ka_ponto_a = kaa.CalculateMeanCurvature();
      }

      array_kam[i] = fabs(ka_ponto_a);
      i++;

      double kb_ponto_b = kab.CalculateGaussCurvature();
      // if (fabs(kb_ponto_b) < 0, 0001) {
      if (fabs(kb_ponto_b) < 0.0001) {
        kb_ponto_b = kab.CalculateMeanCurvature();
      }

      array_kam[i] = fabs(kb_ponto_b);
      i++;

      double kc_ponto_c = kac.CalculateGaussCurvature();
      // if (fabs(kc_ponto_c) < 0, 0001) {
      if (fabs(kc_ponto_c) < 0.0001) {
        kc_ponto_c = kac.CalculateMeanCurvature();
      }

      array_kam[i] = fabs(kc_ponto_c);
      i++;

      double kd_ponto_d = kad.CalculateGaussCurvature();
      // if (fabs(kd_ponto_d) < 0, 0001) {
      if (fabs(kd_ponto_d) < 0.0001) {
        kd_ponto_d = kad.CalculateMeanCurvature();
      }

      array_kam[i] = fabs(kd_ponto_d);
      i++;

      if (points >= 9) {
        PointAdaptive ponto_e = patch->Parameterize(0.5, 0.5);
        PointAdaptive ponto_f = patch->Parameterize(0.0, 0.0);
        PointAdaptive ponto_g = patch->Parameterize(1.0, 0.0);
        PointAdaptive ponto_h = patch->Parameterize(0.0, 1.0);
        PointAdaptive ponto_i = patch->Parameterize(1.0, 1.0);

        CurvatureAnalytical kae(
            static_cast<NodeAdaptive>(ponto_e),
            *(std::dynamic_pointer_cast<PatchCoons>(patch)));
        CurvatureAnalytical kaf(
            static_cast<NodeAdaptive>(ponto_f),
            *(std::dynamic_pointer_cast<PatchCoons>(patch)));
        CurvatureAnalytical kag(
            static_cast<NodeAdaptive>(ponto_g),
            *(std::dynamic_pointer_cast<PatchCoons>(patch)));
        CurvatureAnalytical kah(
            static_cast<NodeAdaptive>(ponto_h),
            *(std::dynamic_pointer_cast<PatchCoons>(patch)));
        CurvatureAnalytical kai(
            static_cast<NodeAdaptive>(ponto_i),
            *(std::dynamic_pointer_cast<PatchCoons>(patch)));

        double ke_ponto_e = kae.CalculateGaussCurvature();
        // if (fabs(ke_ponto_e) < 0, 0001) {
        if (fabs(ke_ponto_e) < 0.0001) {
          ke_ponto_e = kae.CalculateMeanCurvature();
        }

        array_kam[i] = fabs(ke_ponto_e);
        i++;

        double kf_ponto_f = kaf.CalculateGaussCurvature();
        // if (fabs(kf_ponto_f) < 0, 0001) {
        if (fabs(kf_ponto_f) < 0.0001) {
          kf_ponto_f = kaf.CalculateMeanCurvature();
        }

        array_kam[i] = fabs(kf_ponto_f);
        i++;

        double kg_ponto_g = kag.CalculateGaussCurvature();
        // if (fabs(kg_ponto_g) < 0, 0001) {
        if (fabs(kg_ponto_g) < 0.0001) {
          kg_ponto_g = kag.CalculateMeanCurvature();
        }

        array_kam[i] = fabs(kg_ponto_g);
        i++;

        double kh_ponto_h = kah.CalculateGaussCurvature();
        // if (fabs(kh_ponto_h) < 0, 0001) {
        if (fabs(kh_ponto_h) < 0.0001) {
          kh_ponto_h = kah.CalculateMeanCurvature();
        }

        array_kam[i] = fabs(kh_ponto_h);
        i++;

        double ki_ponto_i = kai.CalculateGaussCurvature();
        // if (fabs(ki_ponto_i) < 0, 0001) {
        if (fabs(ki_ponto_i) < 0.0001) {
          ki_ponto_i = kai.CalculateMeanCurvature();
        }

        array_kam[i] = fabs(ki_ponto_i);
        i++;

        if (points >= 17) {
          PointAdaptive ponto_j = patch->Parameterize(0.125, 0.125);
          PointAdaptive ponto_l = patch->Parameterize(0.5, 0.125);
          PointAdaptive ponto_m = patch->Parameterize(0.875, 0.125);
          PointAdaptive ponto_n = patch->Parameterize(0.125, 0.5);
          PointAdaptive ponto_o = patch->Parameterize(0.875, 0.5);
          PointAdaptive ponto_p = patch->Parameterize(0.125, 0.875);
          PointAdaptive ponto_q = patch->Parameterize(0.5, 0.875);
          PointAdaptive ponto_r = patch->Parameterize(0.875, 0.875);

          CurvatureAnalytical kaj(
              static_cast<NodeAdaptive>(ponto_j),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kal(
              static_cast<NodeAdaptive>(ponto_l),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kam(
              static_cast<NodeAdaptive>(ponto_m),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kan(
              static_cast<NodeAdaptive>(ponto_n),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kao(
              static_cast<NodeAdaptive>(ponto_o),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kap(
              static_cast<NodeAdaptive>(ponto_p),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kaq(
              static_cast<NodeAdaptive>(ponto_q),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));
          CurvatureAnalytical kar(
              static_cast<NodeAdaptive>(ponto_r),
              *(std::dynamic_pointer_cast<PatchCoons>(patch)));

          double kj_ponto_j = kaj.CalculateGaussCurvature();
          // if (fabs(kj_ponto_j) < 0, 0001) {
          if (fabs(kj_ponto_j) < 0.0001) {
            kj_ponto_j = kaj.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kj_ponto_j);
          i++;

          double kl_ponto_l = kal.CalculateGaussCurvature();
          // if (fabs(kl_ponto_l) < 0, 0001) {
          if (fabs(kl_ponto_l) < 0.0001) {
            kl_ponto_l = kal.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kl_ponto_l);
          i++;

          double km_ponto_m = kam.CalculateGaussCurvature();
          // if (fabs(km_ponto_m) < 0, 0001) {
          if (fabs(km_ponto_m) < 0.0001) {
            km_ponto_m = kam.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(km_ponto_m);
          i++;

          double kn_ponto_n = kan.CalculateGaussCurvature();
          // if (fabs(kn_ponto_n) < 0, 0001) {
          if (fabs(kn_ponto_n) < 0.0001) {
            kn_ponto_n = kan.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kn_ponto_n);
          i++;

          double ko_ponto_o = kao.CalculateGaussCurvature();
          // if (fabs(ko_ponto_o) < 0, 0001) {
          if (fabs(ko_ponto_o) < 0.0001) {
            ko_ponto_o = kao.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(ko_ponto_o);
          i++;

          double kp_ponto_p = kap.CalculateGaussCurvature();
          // if (fabs(kp_ponto_p) < 0, 0001) {
          if (fabs(kp_ponto_p) < 0.0001) {
            kp_ponto_p = kap.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kp_ponto_p);
          i++;

          double kq_ponto_q = kaq.CalculateGaussCurvature();
          // if (fabs(kq_ponto_q) < 0, 0001) {
          if (fabs(kq_ponto_q) < 0.0001) {
            kq_ponto_q = kaq.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kq_ponto_q);
          i++;

          double kr_ponto_r = kar.CalculateGaussCurvature();
          // if (fabs(kr_ponto_r) < 0, 0001) {
          if (fabs(kr_ponto_r) < 0.0001) {
            kr_ponto_r = kar.CalculateMeanCurvature();
          }

          array_kam[i] = fabs(kr_ponto_r);
          i++;

          if (points == 25) {
            PointAdaptive ponto_s = patch->Parameterize(0.25, 0.0);
            PointAdaptive ponto_t = patch->Parameterize(0.75, 0.0);
            PointAdaptive ponto_u = patch->Parameterize(0.0, 0.25);
            PointAdaptive ponto_v = patch->Parameterize(1.0, 0.25);
            PointAdaptive ponto_w = patch->Parameterize(0.0, 0.75);
            PointAdaptive ponto_x = patch->Parameterize(1.0, 0.75);
            PointAdaptive ponto_y = patch->Parameterize(0.25, 1.0);
            PointAdaptive ponto_z = patch->Parameterize(0.75, 1.0);

            CurvatureAnalytical kas(
                static_cast<NodeAdaptive>(ponto_s),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kat(
                static_cast<NodeAdaptive>(ponto_t),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kau(
                static_cast<NodeAdaptive>(ponto_u),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kav(
                static_cast<NodeAdaptive>(ponto_v),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kaw(
                static_cast<NodeAdaptive>(ponto_w),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kax(
                static_cast<NodeAdaptive>(ponto_x),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kay(
                static_cast<NodeAdaptive>(ponto_y),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));
            CurvatureAnalytical kaz(
                static_cast<NodeAdaptive>(ponto_z),
                *(std::dynamic_pointer_cast<PatchCoons>(patch)));

            double ks_ponto_s = kas.CalculateGaussCurvature();
            // if (fabs(ks_ponto_s) < 0, 0001) {
            if (fabs(ks_ponto_s) < 0.0001) {
              ks_ponto_s = kas.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(ks_ponto_s);
            i++;

            double kt_ponto_t = kat.CalculateGaussCurvature();
            // if (fabs(kt_ponto_t) < 0, 0001) {
            if (fabs(kt_ponto_t) < 0.0001) {
              kt_ponto_t = kat.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(kt_ponto_t);
            i++;

            double ku_ponto_u = kau.CalculateGaussCurvature();
            // if (fabs(ku_ponto_u) < 0, 0001) {
            if (fabs(ku_ponto_u) < 0.0001) {
              ku_ponto_u = kau.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(ku_ponto_u);
            i++;

            double kv_ponto_v = kav.CalculateGaussCurvature();
            // if (fabs(kv_ponto_v) < 0, 0001) {
            if (fabs(kv_ponto_v) < 0.0001) {
              kv_ponto_v = kav.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(kv_ponto_v);
            i++;

            double kw_ponto_w = kaw.CalculateGaussCurvature();
            // if (fabs(kw_ponto_w) < 0, 0001) {
            if (fabs(kw_ponto_w) < 0.0001) {
              kw_ponto_w = kaw.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(kw_ponto_w);
            i++;

            double kx_ponto_x = kax.CalculateGaussCurvature();
            // if (fabs(kx_ponto_x) < 0, 0001) {
            if (fabs(kx_ponto_x) < 0.0001) {
              kx_ponto_x = kax.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(kx_ponto_x);
            i++;

            double ky_ponto_y = kay.CalculateGaussCurvature();
            // if (fabs(ky_ponto_y) < 0, 0001) {
            if (fabs(ky_ponto_y) < 0.0001) {
              ky_ponto_y = kay.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(ky_ponto_y);
            i++;

            double kz_ponto_z = kaz.CalculateGaussCurvature();
            // if (fabs(kz_ponto_z) < 0, 0001) {
            if (fabs(kz_ponto_z) < 0.0001) {
              kz_ponto_z = kaz.CalculateMeanCurvature();
            }

            array_kam[i] = fabs(kz_ponto_z);
            i++;
          }
        }
      }
    }
  }

  double result = 0.0;
  for (int j = 0; j < i; j++) {
    result += array_kam[j];
  }

  return result / points;
}

double ChargeEstimateProcess::CalculateAreaPatch(
    std::shared_ptr<PatchBezier>& patch, int pointsGaussLegandre) {
  VectorAdaptive V;
  double I = 0.0;
  double beta[pointsGaussLegandre];
  double peso[pointsGaussLegandre];
  double u = 0.0, v = 0.0;

  switch (pointsGaussLegandre) {
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

  for (int i = 0; i < pointsGaussLegandre; ++i) {
    u = 0.5 * (1 + beta[i]);

    for (int j = 0; j < pointsGaussLegandre; ++j) {
      v = 0.5 * (1 + beta[j]);

      VectorAdaptive Pu = patch->Qu(u, v);
      VectorAdaptive Pv = patch->Qv(u, v);

      V.SetX(Pu.GetY() * Pv.GetZ() - Pu.GetZ() * Pv.GetY());
      V.SetY(Pu.GetZ() * Pv.GetX() - Pu.GetX() * Pv.GetZ());
      V.SetZ(Pu.GetX() * Pv.GetY() - Pu.GetY() * Pv.GetX());

      I = I +
          0.25 *
              ((sqrt(pow(V.GetX(), 2) + pow(V.GetY(), 2) + pow(V.GetZ(), 2))) *
               peso[i] * peso[j]);
    }
  }

  return I;
}

double ChargeEstimateProcess::CalculateAreaTriangleMedioRad(
    std::shared_ptr<PatchBezier>& patch) {
  double Kam = patch->GetKaMedio();

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

double ChargeEstimateProcess::CalculateAreaTriangleMedio(
    std::shared_ptr<PatchBezier>& patch, Timer& timer, int degree) {
  auto mesh = std::make_shared<MeshAdaptive>();
  auto sub_mesh = InitialMeshEstimate(patch, degree);
  mesh->InsertSubMeshAdaptive(sub_mesh);
  // delete sub;

  while (CalculateErroEstimative(mesh, timer, degree) && degree < 5) {
    ++degree;
    //        cout<<"grau: "<<grau<<endl;
    //        cout<<"sub: "<<sub<<endl;
    auto sub1 = InitialMeshEstimate(patch, degree);
    mesh->RemoveSubMeshAdaptive();
    mesh->InsertSubMeshAdaptive(sub1);
    // delete sub1;
  }

  // cout<<"menor_grau: "<<menor_grau<<endl;

  auto sub2 = InitialMeshEstimate(patch, minor_degree_);
  mesh->RemoveSubMeshAdaptive();
  mesh->InsertSubMeshAdaptive(sub2);

  patch->SetAreaTriangle(
      mesh->GetSubMeshAdaptiveByPosition(0)->GetElement(0)->GetArea());

  return patch->GetAreaTriangle();
}

long ChargeEstimateProcess::CalculateNumbersTriangle(
    std::shared_ptr<PatchBezier>& patch, int degree) {
  auto sub = InitialMeshEstimate(patch, degree);

  return sub->GetNumberElements();
}

// grau tem que ser multiplo de grau == 2^n
std::shared_ptr<SubMesh> ChargeEstimateProcess::InitialMeshEstimate(
    std::shared_ptr<PatchCoons> patch, int degree) {
  int idv = 1;
  int ide = 1;
  int jump = degree;
  int length = degree + 1;
  int total_1 = 0;
  int total_2 = 0;

  auto c1 = patch->GetCurve(0);
  auto c2 = patch->GetCurve(1);
  auto c3 = patch->GetCurve(2);
  auto c4 = patch->GetCurve(3);

  // 1. verifica quais curvas ainda não foram discretizadas
  if (c1->GetNumBerPoints())
    c1 = nullptr;  // c1 já foi trabalhada no patch vizinho
  if (c2->GetNumBerPoints())
    c2 = nullptr;  // c2 já foi trabalhada no patch vizinho
  if (c3->GetNumBerPoints())
    c3 = nullptr;  // c3 já foi trabalhada no patch vizinho
  if (c4->GetNumBerPoints())
    c4 = nullptr;  // c4 já foi trabalhada no patch vizinho

  auto sub = std::make_shared<SubMesh>();

  for (double v = 0.0; v <= 1.0; v += 1.0 / degree) {
    for (double u = 0.0; u <= 1.0; u += 1.0 / degree) {
      auto noh = std::make_shared<NodeAdaptive>(patch->Parameterize(u, v));
      noh->SetId(idv++);

      // cout << "u = " << u << " v = " << v << endl;
      if (v == 0 and c1)  // p está na curva 1
        c1->InsertPoint(noh);
      else if (v == 1 and c3)  // p está na curva 3
        c3->InsertPoint(noh);

      if (u == 0 and c4)  // p está na curva 4
        c4->InsertPoint(noh);
      else if (u == 1 and c2)  // p está na curva 2
        c2->InsertPoint(noh);

      sub->SetNoh(noh);
      total_1++;
    }
  }

  for (double v = 1.0 / (2.0 * degree); v <= 1.0 - (1.0 / (2.0 * degree));
       v += 1.0 / degree) {
    for (double u = 1.0 / (2.0 * degree); u <= 1.0 - (1.0 / (2.0 * degree));
         u += 1.0 / degree) {
      //   cout << "u = " << u << " v = " << v << endl;
      auto noh = std::make_shared<NodeAdaptive>(patch->Parameterize(u, v));
      sub->SetNoh(noh);
      noh->SetId(idv++);
    }
  }

  total_2 = total_1;

  for (int i = 0; i < (total_2 - length - 1); ++i) {
    if (i == jump) {
      jump = jump + length;
      ++i;
      total_1 = total_1 - 1;
    }

    auto e1 = std::make_shared<TriangleAdaptive>(
        sub->GetNoh(i), sub->GetNoh(i + 1), sub->GetNoh(i + total_1));

    e1->SetParametersN1(patch->FindUV(*(sub->GetNoh(i))));
    e1->SetParametersN2(patch->FindUV(*(sub->GetNoh(i + 1))));
    e1->SetParametersN3(patch->FindUV(*(sub->GetNoh(i + total_1))));
    e1->SetId(ide++);
    sub->SetElement(e1);

    auto e2 = std::make_shared<TriangleAdaptive>(sub->GetNoh(i + 1),
                                                 sub->GetNoh(i + length + 1),
                                                 sub->GetNoh(i + total_1));

    e2->SetParametersN1(patch->FindUV(*(sub->GetNoh(i + 1))));
    e2->SetParametersN2(patch->FindUV(*(sub->GetNoh(i + length + 1))));
    e2->SetParametersN3(patch->FindUV(*(sub->GetNoh(i + total_1))));
    e2->SetId(ide++);
    sub->SetElement(e2);

    auto e3 = std::make_shared<TriangleAdaptive>(sub->GetNoh(i + length + 1),
                                                 sub->GetNoh(i + length),
                                                 sub->GetNoh(i + total_1));

    e3->SetParametersN1(patch->FindUV(*(sub->GetNoh(i + length + 1))));
    e3->SetParametersN2(patch->FindUV(*(sub->GetNoh(i + length))));
    e3->SetParametersN3(patch->FindUV(*(sub->GetNoh(i + total_1))));
    e3->SetId(ide++);
    sub->SetElement(e3);

    auto e4 = std::make_shared<TriangleAdaptive>(
        sub->GetNoh(i + length), sub->GetNoh(i), sub->GetNoh(i + total_1));

    e4->SetParametersN1(patch->FindUV(*(sub->GetNoh(i + length))));
    e4->SetParametersN2(patch->FindUV(*(sub->GetNoh(i))));
    e4->SetParametersN3(patch->FindUV(*(sub->GetNoh(i + total_1))));
    e4->SetId(ide++);
    sub->SetElement(e4);
  }

  // 5. define a submalha do patch
  patch->SetSubMesh(sub);
  sub->SetPatch(patch);

  auto mesh = std::make_shared<MeshAdaptive>();
  mesh->InsertSubMeshAdaptive(sub);

  return sub;
}

bool ChargeEstimateProcess::CalculateErroEstimative(
    std::shared_ptr<MeshAdaptive>& mesh, Timer& timer, int degree) {
  auto ger = std::make_shared<GeneratorAdaptive>();
#if USE_OPENMP
  double erro = ger->CalculateErrorGlobalOmp(mesh, timer);
#else
  double erro = ger->ErrorGlobal(mesh, timer);
#endif
  // delete ger;

  // cout << "erro: " << erro << " tolerancia: " << kEstimativeTolerance <<
  // endl;

  if (erro < minor_error_) {
    minor_degree_ = degree;
  }

  if (erro <= kEstimativeTolerance) {
    //  cout<<"return: false" <<endl;
    return false;
  }
  // cout<<"return: true" <<endl;

  return true;
}
