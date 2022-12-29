#include "../../../include/data/patch/patch_bezier.h"

extern double TOLERANCIA;  // distância máxima entre dois pontos
extern double I_MAX;

PatchBezier::PatchBezier() : PatchCoons() {}

PatchBezier::PatchBezier(PatchBezier* patch_bezier) : PatchCoons(patch_bezier) {
  this->pt03_ = patch_bezier->pt03_;
  this->pt13_ = patch_bezier->pt13_;
  this->pt23_ = patch_bezier->pt23_;
  this->pt33_ = patch_bezier->pt33_;
  this->pt02_ = patch_bezier->pt02_;
  this->pt12_ = patch_bezier->pt12_;
  this->pt22_ = patch_bezier->pt22_;
  this->pt32_ = patch_bezier->pt32_;
  this->pt01_ = patch_bezier->pt01_;
  this->pt11_ = patch_bezier->pt11_;
  this->pt21_ = patch_bezier->pt21_;
  this->pt31_ = patch_bezier->pt31_;
  this->pt00_ = patch_bezier->pt00_;
  this->pt10_ = patch_bezier->pt10_;
  this->pt20_ = patch_bezier->pt20_;
  this->pt30_ = patch_bezier->pt30_;

  this->mat_geo_gx_ = patch_bezier->mat_geo_gx_;
  this->mat_geo_gy_ = patch_bezier->mat_geo_gy_;
  this->mat_geo_gz_ = patch_bezier->mat_geo_gz_;
  this->mat_base_u_ = patch_bezier->mat_base_u_;
  this->mat_base_v_ = patch_bezier->mat_base_v_;
}

// Ordem das curvas:
//		C3
//	C4		C2
//		C1
PatchBezier::PatchBezier(CurveAdaptive* curve1, CurveAdaptive* curve2,
                         CurveAdaptive* curve3, CurveAdaptive* curve4,
                         Ponto pt11, Ponto pt21, Ponto pt12, Ponto pt22,
                         bool signal_curve1, bool signal_curve2,
                         bool signal_curve3, bool signal_curve4)
    : PatchCoons() {
  this->signal_curve1_ = signal_curve1;
  this->signal_curve2_ = signal_curve2;
  this->signal_curve3_ = signal_curve3;
  this->signal_curve4_ = signal_curve4;

  // As curvas devem ser definidas da esquerda para a direita, de baixo para
  // cima em relação ao Patch !!!
  // 1. Inclui as curvas na lista de curvas de CoonsPatch
  //
  this->curves_.push_back(curve1);
  this->curves_.push_back(curve2);
  this->curves_.push_back(curve3);
  this->curves_.push_back(curve4);
  //
  // 2. Coloca o Patch na lista das curvas
  //
  static_cast<CurveAdaptiveParametricBezier*>(curve1)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricBezier*>(curve2)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricBezier*>(curve3)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricBezier*>(curve4)->InsertPatch(this);
  //
  // 3. Seta os atributos de acordo com as curvas
  //
  this->pt00_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve1)->GetPoint0();
  this->pt10_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve1)->GetPoint1();
  this->pt20_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve1)->GetPoint2();

  this->pt30_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve2)->GetPoint0();
  this->pt31_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve2)->GetPoint1();
  this->pt32_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve2)->GetPoint2();

  this->pt13_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve3)->GetPoint1();
  this->pt23_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve3)->GetPoint2();
  this->pt33_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve3)->GetPoint3();

  this->pt01_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve4)->GetPoint1();
  this->pt02_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve4)->GetPoint2();
  this->pt03_ =
      static_cast<CurveAdaptiveParametricBezier*>(curve4)->GetPoint3();

  this->pt11_ = pt11;
  this->pt21_ = pt21;
  this->pt12_ = pt12;
  this->pt22_ = pt22;
  //
  // 4. Aloca espaço para as matrizes
  //
  this->mat_base_u_.setZero(1, 4);
  this->mat_base_v_.setZero(4, 1);
  this->mat_geo_gx_.setZero(4, 4);
  this->mat_geo_gy_.setZero(4, 4);
  this->mat_geo_gz_.setZero(4, 4);
  //
  // 5. Preenche as matrizes geométricas com G de Bezier
  //
  // Gx:
  // 2x2 superior esquerdo
  mat_geo_gx_(0, 0) = this->pt00_.x;
  mat_geo_gx_(0, 1) = this->pt01_.x;
  mat_geo_gx_(1, 0) = this->pt10_.x;
  mat_geo_gx_(1, 1) = this->pt11_.x;
  // 2x2 inferior esquerdo
  mat_geo_gx_(2, 0) = this->pt20_.x;
  mat_geo_gx_(2, 1) = this->pt21_.x;
  mat_geo_gx_(3, 0) = this->pt30_.x;
  mat_geo_gx_(3, 1) = this->pt31_.x;
  // 2x2 superior direito
  mat_geo_gx_(0, 2) = this->pt02_.x;
  mat_geo_gx_(0, 3) = this->pt03_.x;
  mat_geo_gx_(1, 2) = this->pt12_.x;
  mat_geo_gx_(1, 3) = this->pt13_.x;
  // 2x2 inferior direito
  mat_geo_gx_(2, 2) = this->pt22_.x;
  mat_geo_gx_(2, 3) = this->pt23_.x;
  mat_geo_gx_(3, 2) = this->pt32_.x;
  mat_geo_gx_(3, 3) = this->pt33_.x;
  //
  // Gy:
  // 2x2 superior esquerdo
  this->mat_geo_gy_(0, 0) = this->pt00_.y;
  mat_geo_gy_(0, 1) = this->pt01_.y;
  mat_geo_gy_(1, 0) = this->pt10_.y;
  mat_geo_gy_(1, 1) = this->pt11_.y;
  // 2x2 inferior esquerdo
  mat_geo_gy_(2, 0) = this->pt20_.y;
  mat_geo_gy_(2, 1) = this->pt21_.y;
  mat_geo_gy_(3, 0) = this->pt30_.y;
  mat_geo_gy_(3, 1) = this->pt31_.y;
  // 2x2 superior direito
  mat_geo_gy_(0, 2) = this->pt02_.y;
  mat_geo_gy_(0, 3) = this->pt03_.y;
  mat_geo_gy_(1, 2) = this->pt12_.y;
  mat_geo_gy_(1, 3) = this->pt13_.y;
  // 2x2 inferior direito
  mat_geo_gy_(2, 2) = this->pt22_.y;
  mat_geo_gy_(2, 3) = this->pt23_.y;
  mat_geo_gy_(3, 2) = this->pt32_.y;
  mat_geo_gy_(3, 3) = this->pt33_.y;
  //
  // Gz:
  // 2x2 superior esquerdo
  mat_geo_gz_(0, 0) = this->pt00_.z;
  mat_geo_gz_(0, 1) = this->pt01_.z;
  mat_geo_gz_(1, 0) = this->pt10_.z;
  mat_geo_gz_(1, 1) = this->pt11_.z;
  // 2x2 inferior esquerdo
  mat_geo_gz_(2, 0) = this->pt20_.z;
  mat_geo_gz_(2, 1) = this->pt21_.z;
  mat_geo_gz_(3, 0) = this->pt30_.z;
  mat_geo_gz_(3, 1) = this->pt31_.z;
  // 2x2 superior direito
  mat_geo_gz_(0, 2) = this->pt02_.z;
  mat_geo_gz_(0, 3) = this->pt03_.z;
  mat_geo_gz_(1, 2) = this->pt12_.z;
  mat_geo_gz_(1, 3) = this->pt13_.z;
  // 2x2 inferior direito
  mat_geo_gz_(2, 2) = this->pt22_.z;
  mat_geo_gz_(2, 3) = this->pt23_.z;
  mat_geo_gz_(3, 2) = this->pt32_.z;
  mat_geo_gz_(3, 3) = this->pt33_.z;
  //
  // 6.Preenche a Matriz de Bezier
  //
  this->mat_base_ = StartBezierMatrix();
  //
  this->mat_geo_gx_ = this->GetB() * this->GetGx() * this->GetB();
  this->mat_geo_gy_ = this->GetB() * this->GetGy() * this->GetB();
  this->mat_geo_gz_ = this->GetB() * this->GetGz() * this->GetB();
}

// Esse patch acha que é isolado! As curvas que apontam para ele devem ser
// definidas externamente.
PatchBezier ::PatchBezier(Ponto pt00, Ponto pt01, Ponto pt02, Ponto pt03,
                          Ponto pt10, Ponto pt11, Ponto pt12, Ponto pt13,
                          Ponto pt20, Ponto pt21, Ponto pt22, Ponto pt23,
                          Ponto pt30, Ponto pt31, Ponto pt32, Ponto pt33,
                          bool signal_curve1, bool signal_curve2,
                          bool signal_curve3, bool signal_curve4)
    : PatchCoons(),
      pt03_(pt03),
      pt13_(pt13),
      pt23_(pt23),
      pt33_(pt33),
      pt02_(pt02),
      pt12_(pt12),
      pt22_(pt22),
      pt32_(pt32),
      pt01_(pt01),
      pt11_(pt11),
      pt21_(pt21),
      pt31_(pt31),
      pt00_(pt00),
      pt10_(pt10),
      pt20_(pt20),
      pt30_(pt30) {
  this->signal_curve1_ = signal_curve1;
  this->signal_curve2_ = signal_curve2;
  this->signal_curve3_ = signal_curve3;
  this->signal_curve4_ = signal_curve4;

  // 1. Inclui as curvas na lista de curvas de CoonsPatch
  //
  this->curves_.push_back(
      new CurveAdaptiveParametricBezier(pt00_, pt10_, pt20_, pt30_));
  this->curves_.push_back(
      new CurveAdaptiveParametricBezier(pt30_, pt31_, pt32_, pt33_));
  this->curves_.push_back(
      new CurveAdaptiveParametricBezier(pt03_, pt13_, pt23_, pt33_));
  this->curves_.push_back(
      new CurveAdaptiveParametricBezier(pt00_, pt01_, pt02_, pt03_));
  //
  // 2. Aloca espaço para as matrizes
  //
  this->mat_base_u_.setZero(1, 4);
  this->mat_base_v_.setZero(4, 1);
  this->mat_geo_gx_.setZero(4, 4);
  this->mat_geo_gy_.setZero(4, 4);
  this->mat_geo_gz_.setZero(4, 4);
  //
  ////////////////////////////////////

  // 3. Preenche as matrizes geométricas com G de Bezier
  //
  // 2x2 superior esquerdo
  mat_geo_gx_(0, 0) = this->pt00_.x;
  mat_geo_gx_(0, 1) = this->pt01_.x;
  mat_geo_gx_(1, 0) = this->pt10_.x;
  mat_geo_gx_(1, 1) = this->pt11_.x;
  // 2x2 inferior esquerdo
  mat_geo_gx_(2, 0) = this->pt20_.x;
  mat_geo_gx_(2, 1) = this->pt21_.x;
  mat_geo_gx_(3, 0) = this->pt30_.x;
  mat_geo_gx_(3, 1) = this->pt31_.x;
  // 2x2 superior direito
  mat_geo_gx_(0, 2) = this->pt02_.x;
  mat_geo_gx_(0, 3) = this->pt03_.x;
  mat_geo_gx_(1, 2) = this->pt12_.x;
  mat_geo_gx_(1, 3) = this->pt13_.x;
  // 2x2 inferior direito
  mat_geo_gx_(2, 2) = this->pt22_.x;
  mat_geo_gx_(2, 3) = this->pt23_.x;
  mat_geo_gx_(3, 2) = this->pt32_.x;
  mat_geo_gx_(3, 3) = this->pt33_.x;
  //
  // Gy:
  // 2x2 superior esquerdo
  mat_geo_gy_(0, 0) = this->pt00_.y;
  mat_geo_gy_(0, 1) = this->pt01_.y;
  mat_geo_gy_(1, 0) = this->pt10_.y;
  mat_geo_gy_(1, 1) = this->pt11_.y;
  // 2x2 inferior esquerdo
  mat_geo_gy_(2, 0) = this->pt20_.y;
  mat_geo_gy_(2, 1) = this->pt21_.y;
  mat_geo_gy_(3, 0) = this->pt30_.y;
  mat_geo_gy_(3, 1) = this->pt31_.y;
  // 2x2 superior direito
  mat_geo_gy_(0, 2) = this->pt02_.y;
  mat_geo_gy_(0, 3) = this->pt03_.y;
  mat_geo_gy_(1, 2) = this->pt12_.y;
  mat_geo_gy_(1, 3) = this->pt13_.y;
  // 2x2 inferior direito
  mat_geo_gy_(2, 2) = this->pt22_.y;
  mat_geo_gy_(2, 3) = this->pt23_.y;
  mat_geo_gy_(3, 2) = this->pt32_.y;
  mat_geo_gy_(3, 3) = this->pt33_.y;
  //
  // Gz:
  // 2x2 superior esquerdo
  mat_geo_gz_(0, 0) = this->pt00_.z;
  mat_geo_gz_(0, 1) = this->pt01_.z;
  mat_geo_gz_(1, 0) = this->pt10_.z;
  mat_geo_gz_(1, 1) = this->pt11_.z;
  // 2x2 inferior esquerdo
  mat_geo_gz_(2, 0) = this->pt20_.z;
  mat_geo_gz_(2, 1) = this->pt21_.z;
  mat_geo_gz_(3, 0) = this->pt30_.z;
  mat_geo_gz_(3, 1) = this->pt31_.z;
  // 2x2 superior direito
  mat_geo_gz_(0, 2) = this->pt02_.z;
  mat_geo_gz_(0, 3) = this->pt03_.z;
  mat_geo_gz_(1, 2) = this->pt12_.z;
  mat_geo_gz_(1, 3) = this->pt13_.z;
  // 2x2 inferior direito
  mat_geo_gz_(2, 2) = this->pt22_.z;
  mat_geo_gz_(2, 3) = this->pt23_.z;
  mat_geo_gz_(3, 2) = this->pt32_.z;
  mat_geo_gz_(3, 3) = this->pt33_.z;
  //
  // 6.Preenche a Matriz de Bezier
  //
  this->mat_base_ = StartBezierMatrix();
  //
  this->mat_geo_gx_ = this->GetB() * this->GetGx() * this->GetB();
  this->mat_geo_gy_ = this->GetB() * this->GetGy() * this->GetB();
  this->mat_geo_gz_ = this->GetB() * this->GetGz() * this->GetB();
}

PatchBezier::~PatchBezier() {
  delete &mat_base_v_;
  delete &mat_base_u_;
}

Ponto PatchBezier::CalculatePointUV() {
  Ponto point;

  // C = ( U * ( B * ( G * ( Bt * V ) ) ) )
  point.x = (this->GetU() * ((this->GetGx() * (this->GetV()))))(0, 0);
  point.y = (this->GetU() * ((this->GetGy() * (this->GetV()))))(0, 0);
  point.z = (this->GetU() * ((this->GetGz() * (this->GetV()))))(0, 0);

  return point;
}

void PatchBezier::PrintAllMatrixPatchBezier() {
  cout << "U:\n" << mat_base_u_;
  cout << "B:\n" << mat_base_;
  cout << "Gx:\n" << mat_geo_gx_;
  cout << "Gy:\n" << mat_geo_gy_;
  cout << "Gz:\n" << mat_geo_gz_;
  cout << "V:\n" << mat_base_v_;
}

// encontra o parâmetro t de um dado ponto p na curva
tuple<double, double> PatchBezier::FindUV(const Ponto& p) {
  unsigned int iMax = 0;

  // chute inicial
  double u_i = 0.5;
  double v_i = 0.5;
  ////////////////

  double delta_u = 0.0;  // o quanto o parâmetro terá de percorrer
  double delta_v = 0.0;  // o quanto o parâmetro terá de percorrer

  // Método de Jacobi para resolução de sistema
  Ponto p_i;
  // cout << "FindUV (" << p.id << "), usando Jacobi!";

  do {
    // #pragma omp critical
    //         cout<<"u_i: "<<u_i<<" v_i: "<<v_i<<" thread:
    //         "<<omp_get_thread_num()<<endl;

    Vetor Tu = -(this->Qu(u_i, v_i));
    Vetor Tv = -(this->Qv(u_i, v_i));

    p_i = this->Parameterize(u_i, v_i);  // palpite inicial

    //        if (std::isnan(p_i.x)) {
    // #pragma omp critical
    //            {
    //                cout<<"-nan pi"<<endl;
    //                cout<<"u_i: "<<u_i<<" v_i: "<<v_i<<" thread:
    //                "<<omp_get_thread_num()<<endl; cout<< p_i.id << " (" <<
    //                p.x << ", " << p.y << ", " << p.z << ") thread:
    //                "<<omp_get_thread_num()<< endl;
    //            }
    //        }

    Matrix<double, 3, 3> matrix;
    matrix(0, 0) = Tu.x;
    matrix(0, 1) = Tv.x;
    matrix(0, 2) = p_i.x - p.x;
    matrix(1, 0) = Tu.y;
    matrix(1, 1) = Tv.y;
    matrix(1, 2) = p_i.y - p.y;
    matrix(2, 0) = Tu.z;
    matrix(2, 1) = Tv.z;
    matrix(2, 2) = p_i.z - p.z;

    //        Vector3d b= {p.x, p.y, p.z};
    //        cout<<A.colPivHouseholderQr().solve(b)<<endl;

    int k = 0;
    double pivot = matrix(0, 0);

    while ((fabs(pivot) < TOLERANCIA) and (k < 2)) {
      ++k;
      pivot = matrix(k, 0);
    }

    matrix.row(k).swap(matrix.row(0));

    if (fabs(pivot) < TOLERANCIA) {
      cout << "Erro! Não é possível encontrar as coordenadas paramétricas no "
              "ponto p"
           << p.id << " (" << p.x << ", " << p.y << ", " << p.z << ")" << endl;

      return make_tuple(-1.0, -1.0);
    }

    double A_10 = matrix(1, 0);
    double A_20 = matrix(2, 0);

    for (short j = 0; j < 3; ++j) {
      matrix(0, j) = static_cast<double>(matrix(0, j)) / pivot;
      matrix(1, j) = matrix(1, j) - A_10 * (matrix(0, j));
      matrix(2, j) = matrix(2, j) - A_20 * (matrix(0, j));
    }

    pivot = matrix(1, 1);

    if (fabs(pivot) < TOLERANCIA) {
      pivot = matrix(2, 1);
      matrix.row(2).swap(matrix.row(1));
    }

    double A_01 = matrix(0, 1);
    double A_21 = matrix(2, 1);

    for (short j = 0; j < 3; ++j) {
      matrix(1, j) = static_cast<double>(matrix(1, j) / pivot);
      matrix(0, j) = matrix(0, j) - A_01 * (matrix(1, j));
      matrix(2, j) = matrix(2, j) - A_21 * (matrix(1, j));
    }

    delta_u = matrix(0, 2);
    delta_v = matrix(1, 2);

    //        if (std::isnan(delta_u) || std::isnan(delta_v)){
    // #pragma omp critical
    //            {
    //                cout<<"-nan delta_u e delta_v"<<endl;
    //                cout<<"delta_u: "<<delta_u<<" delta_v: "<<delta_v<<"
    //                thread: "<<omp_get_thread_num()<<endl;
    //            }
    //        }

    if (!std::isnan(delta_u)) {
      u_i += delta_u;
    }

    if (!std::isnan(delta_v)) {
      v_i += delta_v;
    }

    if (++iMax > I_MAX) {
#if USE_PRINT_COMENT
      cout << "iMax alcançado!" << endl;
#endif  // #if USE_PRINT_COMENT

      break;
    }
    // cout << "u = " << u_i << " " << "v = " << v_i << endl;
    // cout << "delta_u = " << delta_u << " " << "delta_v = " << delta_v <<
    // endl;
  } while (fabs(delta_u) >= TOLERANCIA or fabs(delta_v) >= TOLERANCIA);
  // while ( p.distanciaPara(p_i) >= TOLERANCIA );

  if (u_i <= TOLERANCIA)
    u_i = 0.0;
  else if (u_i >= 1.0 - TOLERANCIA)
    u_i = 1.0;

  if (v_i <= TOLERANCIA)
    v_i = 0.0;
  else if (v_i >= 1.0 - TOLERANCIA)
    v_i = 1.0;

  if (std::isnan(u_i) || std::isnan(v_i)) {
    cout << "-nan u_i e v_i" << endl;
  }

  return make_tuple(u_i, v_i);
}

// encontra as coordenadas 3D de um ponto p de parâmetros u, v
Ponto PatchBezier::Parameterize(double u, double v) {
  //	cout << "Parameterize ( " << u << ", " << v << ")" << endl;
  // Parameterize:
  //
  //  -> ALTERA a matriz U
  //  -> ALTERA a matriz V
  //  -> usa 'calculaPonto_u_v ( )'
  //

  this->mat_base_u_(0, 0) = u * u * u;
  this->mat_base_u_(0, 1) = u * u;
  this->mat_base_u_(0, 2) = u;
  this->mat_base_u_(0, 3) = 1;

  this->mat_base_v_(0, 0) = v * v * v;
  this->mat_base_v_(1, 0) = v * v;
  this->mat_base_v_(2, 0) = v;
  this->mat_base_v_(3, 0) = 1;

  return CalculatePointUV();
}

// calcula o vetor tangente na direção u nas coordenadas paramétricas u, v
Vetor PatchBezier::Qu(double u, double v) {
  // Qt:
  //
  //  -> ALOCA um Ponto (mas destroi aqui mesmo)
  //	 -> ALOCA um Vetor (retorna ele)
  //  -> ALTERA a matriz U !!!
  //  -> ALTERA a matriz V !!!
  //

  Ponto P;

  this->mat_base_u_(0, 0) = 3 * u * u;
  this->mat_base_u_(0, 1) = 2 * u;
  this->mat_base_u_(0, 2) = 1;
  this->mat_base_u_(0, 3) = 0;

  this->mat_base_v_(0, 0) = v * v * v;
  this->mat_base_v_(1, 0) = v * v;
  this->mat_base_v_(2, 0) = v;
  this->mat_base_v_(3, 0) = 1;

  P = CalculatePointUV();
  Vetor V(P);

  return V;
}

// calcula o vetor tangente na direção v nas coordenadas paramétricas u, v
Vetor PatchBezier::Qv(double u, double v) {
  // Qt:
  //
  //  -> ALOCA um Ponto (mas destroi aqui mesmo)
  //	 -> ALOCA um Vetor (retorna ele)
  //  -> ALTERA a matriz U !!!
  //  -> ALTERA a matriz V !!!
  //

  Ponto P;

  this->mat_base_u_(0, 0) = u * u * u;
  this->mat_base_u_(0, 1) = u * u;
  this->mat_base_u_(0, 2) = u;
  this->mat_base_u_(0, 3) = 1;

  this->mat_base_v_(0, 0) = 3 * v * v;
  this->mat_base_v_(1, 0) = 2 * v;
  this->mat_base_v_(2, 0) = 1;
  this->mat_base_v_(3, 0) = 0;

  P = CalculatePointUV();
  Vetor V(P);

  return V;
}

// calcula a derivada parcial Quu nas coordenadas paramétricas u, v
Vetor PatchBezier::Quu(double u, double v) {
  // Qt:
  //
  //  -> ALOCA um Ponto (mas destroi aqui mesmo)
  //	 -> ALOCA um Vetor (retorna ele)
  //  -> ALTERA a matriz U !!!
  //  -> ALTERA a matriz V !!!
  //

  Ponto P;

  this->mat_base_u_(0, 0) = 6 * u;
  this->mat_base_u_(0, 1) = 2;
  this->mat_base_u_(0, 2) = 0;
  this->mat_base_u_(0, 3) = 0;

  this->mat_base_v_(0, 0) = v * v * v;
  this->mat_base_v_(1, 0) = v * v;
  this->mat_base_v_(2, 0) = v;
  this->mat_base_v_(3, 0) = 1;

  P = CalculatePointUV();
  Vetor V(P);

  return V;
}

// calcula a derivada parcial Quv nas coordenadas paramétricas u, v
Vetor PatchBezier::Quv(double u, double v) {
  // Qt:
  //
  //  -> ALOCA um Ponto (mas destroi aqui mesmo)
  //	 -> ALOCA um Vetor (retorna ele)
  //  -> ALTERA a matriz U !!!
  //  -> ALTERA a matriz V !!!
  //

  Ponto P;

  this->mat_base_u_(0, 0) = 3 * u * u;
  this->mat_base_u_(0, 1) = 2 * u;
  this->mat_base_u_(0, 2) = 1;
  this->mat_base_u_(0, 3) = 0;

  this->mat_base_v_(0, 0) = 3 * v * v;
  this->mat_base_v_(1, 0) = 2 * v;
  this->mat_base_v_(2, 0) = 1;
  this->mat_base_v_(3, 0) = 0;

  P = CalculatePointUV();
  Vetor V(P);

  return V;
}

// calcula a derivada parcial Qvu nas coordenadas paramétricas u, v
Vetor PatchBezier::Qvu(double u, double v) { return Quv(u, v); }

// calcula a derivada parcial Qvv nas coordenadas paramétricas u, v
Vetor PatchBezier::Qvv(double u, double v) {
  // Qt:
  //
  //  -> ALOCA um Ponto (mas destroi aqui mesmo)
  //	 -> ALOCA um Vetor (retorna ele)
  //  -> ALTERA a matriz U !!!
  //  -> ALTERA a matriz V !!!
  //

  Ponto P;

  this->mat_base_u_(0, 0) = u * u * u;
  this->mat_base_u_(0, 1) = u * u;
  this->mat_base_u_(0, 2) = u;
  this->mat_base_u_(0, 3) = 1;

  this->mat_base_v_(0, 0) = 6 * v;
  this->mat_base_v_(1, 0) = 2;
  this->mat_base_v_(2, 0) = 0;
  this->mat_base_v_(3, 0) = 0;

  P = CalculatePointUV();
  Vetor V(P);

  return V;
}

// calcula o vetor tangente na direção u para o ponto p
Vetor PatchBezier::Qu(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Qu(get<0>(t), get<1>(t));
}

// calcula o vetor tangente na direção v para o ponto p
Vetor PatchBezier::Qv(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Qv(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Quu para o ponto p
Vetor PatchBezier::Quu(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Quu(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Quv para o ponto p
Vetor PatchBezier::Quv(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Quv(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Qvu para o ponto p
Vetor PatchBezier::Qvu(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Qvu(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Qvv para o ponto p
Vetor PatchBezier::Qvv(const Ponto& p) {
  tuple<double, double> t = this->FindUV(p);
  return this->Qvv(get<0>(t), get<1>(t));
}

Matrix1x4 PatchBezier::GetU() const { return this->mat_base_u_; }

Matrix4x4 PatchBezier::GetGx() const { return this->mat_geo_gx_; }

Matrix4x4 PatchBezier::GetGy() const { return this->mat_geo_gy_; }

Matrix4x4 PatchBezier::GetGz() const { return this->mat_geo_gz_; }

Matrix4x1 PatchBezier::GetV() const { return this->mat_base_v_; }

Matrix4x4 PatchBezier::GetB() const { return this->mat_base_; }

Ponto PatchBezier::GetPt00() const { return this->pt00_; }

Ponto PatchBezier::GetPt01() const { return this->pt01_; }

Ponto PatchBezier::GetPt02() const { return this->pt02_; }

Ponto PatchBezier::GetPt03() const { return this->pt03_; }

Ponto PatchBezier::GetPt10() const { return this->pt10_; }

Ponto PatchBezier::GetPt11() const { return this->pt11_; }

Ponto PatchBezier::GetPt12() const { return this->pt12_; }

Ponto PatchBezier::GetPt13() const { return this->pt13_; }

Ponto PatchBezier::GetPt20() const { return this->pt20_; }

Ponto PatchBezier::GetPt21() const { return this->pt21_; }

Ponto PatchBezier::GetPt22() const { return this->pt22_; }

Ponto PatchBezier::GetPt23() const { return this->pt23_; }

Ponto PatchBezier::GetPt30() const { return this->pt30_; }

Ponto PatchBezier::GetPt31() const { return this->pt31_; }

Ponto PatchBezier::GetPt32() const { return this->pt32_; }

Ponto PatchBezier::GetPt33() const { return this->pt33_; }

void PatchBezier::SetPt00(Ponto value) { pt00_ = value; }

void PatchBezier::SetPt01(Ponto value) { pt01_ = value; }

void PatchBezier::SetPt02(Ponto value) { pt02_ = value; }

void PatchBezier::SetPt03(Ponto value) { pt03_ = value; }

void PatchBezier::SetPt10(Ponto value) { pt10_ = value; }

void PatchBezier::SetPt11(Ponto value) { pt11_ = value; }

void PatchBezier::SetPt12(Ponto value) { pt12_ = value; }

void PatchBezier::SetPt13(Ponto value) { pt13_ = value; }

void PatchBezier::SetPt20(Ponto value) { pt20_ = value; }

void PatchBezier::SetPt21(Ponto value) { pt21_ = value; }

void PatchBezier::SetPt22(Ponto value) { pt22_ = value; }

void PatchBezier::SetPt23(Ponto value) { pt23_ = value; }

void PatchBezier::SetPt30(Ponto value) { pt30_ = value; }

void PatchBezier::SetPt31(Ponto value) { pt31_ = value; }

void PatchBezier::SetPt32(Ponto value) { pt32_ = value; }

void PatchBezier::SetPt33(Ponto value) { pt33_ = value; }

double PatchBezier::getNumberTriangle() const { return number_triangle_; }

void PatchBezier::setNumberTriangle(double value) { number_triangle_ = value; }

double PatchBezier::getAreaTriangle() const { return area_triangle_; }

void PatchBezier::setAreaTriangle(double value) { area_triangle_ = value; }

double PatchBezier::getSegmentMedio() const { return segment_medio_; }

void PatchBezier::setSegmentMedio(double value) { segment_medio_ = value; }

double PatchBezier::getKaMedio() const { return ka_medio_; }

void PatchBezier::setKaMedio(double value) { ka_medio_ = value; }

double PatchBezier::getArea() const { return area_; }

void PatchBezier::setArea(double value) { area_ = value; }

int PatchBezier::getIdProcess() const { return id_process_; }

void PatchBezier::setIdProcess(int value) { id_process_ = value; }

int PatchBezier::getId_bezierPatch() const { return id_patch_bezier_; }

void PatchBezier::setId_bezierPatch(int value) { id_patch_bezier_ = value; }

Matrix4x4 PatchBezier::StartBezierMatrix() {
  Matrix4x4 matrix_bezier;

  matrix_bezier(0, 0) = -1;
  matrix_bezier(0, 1) = 3;
  matrix_bezier(0, 2) = -3;
  matrix_bezier(0, 3) = 1;
  matrix_bezier(1, 0) = 3;
  matrix_bezier(1, 1) = -6;
  matrix_bezier(1, 2) = 3;
  matrix_bezier(1, 3) = 0;
  matrix_bezier(2, 0) = -3;
  matrix_bezier(2, 1) = 3;
  matrix_bezier(2, 2) = 0;
  matrix_bezier(2, 3) = 0;
  matrix_bezier(3, 0) = 1;
  matrix_bezier(3, 1) = 0;
  matrix_bezier(3, 2) = 0;
  matrix_bezier(3, 3) = 0;

  return matrix_bezier;
}