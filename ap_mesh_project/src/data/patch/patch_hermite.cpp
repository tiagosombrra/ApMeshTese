#include "../../../include/data/patch/patch_hermite.h"

extern double TOLERANCIA;  // distância máxima entre dois pontos
extern double I_MAX;

PatchHermite::PatchHermite() : PatchCoons() {}

PatchHermite::PatchHermite(PatchHermite* patch_hermite)
    : PatchCoons(patch_hermite) {
  this->pt00_ = patch_hermite->pt00_;
  this->pt01_ = patch_hermite->pt01_;
  this->qv00_ = patch_hermite->qv00_;
  this->qv01_ = patch_hermite->qv01_;
  this->pt10_ = patch_hermite->pt10_;
  this->pt11_ = patch_hermite->pt11_;
  this->qv10_ = patch_hermite->qv10_;
  this->qv11_ = patch_hermite->qv11_;
  this->qu00_ = patch_hermite->qu00_;
  this->qu01_ = patch_hermite->qu01_;
  this->tw00_ = patch_hermite->tw00_;
  this->tw01_ = patch_hermite->tw01_;
  this->qu10_ = patch_hermite->qu10_;
  this->qu11_ = patch_hermite->qu11_;
  this->tw10_ = patch_hermite->tw10_;
  this->tw11_ = patch_hermite->tw11_;

  this->mat_base_ = StartHermiteMatrix();

  this->mat_geo_gx_ = patch_hermite->mat_geo_gx_;
  this->mat_geo_gy_ = patch_hermite->mat_geo_gy_;
  this->mat_geo_gz_ = patch_hermite->mat_geo_gz_;
  this->mat_base_u_ = patch_hermite->mat_base_u_;
  this->mat_base_v_ = patch_hermite->mat_base_v_;
}

// Ordem das curvas:
//		C3
//	C4		C2
//		C1
PatchHermite::PatchHermite(CurveAdaptive* curve1, CurveAdaptive* curve2,
                           CurveAdaptive* curve3, CurveAdaptive* curve4,
                           Vetor tw00, Vetor tw10, Vetor tw01, Vetor tw11,
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
  //////////////////////////////

  // 2. Coloca o Patch na lista das curvas
  //
  static_cast<CurveAdaptiveParametricHermite*>(curve1)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricHermite*>(curve2)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricHermite*>(curve3)->InsertPatch(this);
  static_cast<CurveAdaptiveParametricHermite*>(curve4)->InsertPatch(this);

  // 3. Seta os atributos de acordo com as curvas
  //
  this->pt00_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve1)->GetPoint0();
  this->pt01_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve3)->GetPoint0();
  this->pt10_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve1)->GetPoint1();
  this->pt11_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve3)->GetPoint1();

  this->qv00_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve4)->GetVector0();
  this->qv01_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve4)->GetVector1();
  this->qv10_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve2)->GetVector0();
  this->qv11_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve2)->GetVector1();

  this->qu00_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve1)->GetVector0();
  this->qu01_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve3)->GetVector0();
  this->qu10_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve1)->GetVector1();
  this->qu11_ =
      static_cast<CurveAdaptiveParametricHermite*>(curve3)->GetVector1();

  this->tw00_ = tw00;
  this->tw01_ = tw01;
  this->tw10_ = tw10;
  this->tw11_ = tw11;

  //
  ///////////////////////////////////////

  // 4. Aloca espaço para as matrizes
  //
  this->mat_base_u_.setZero(1, 4);
  this->mat_base_v_.setZero(4, 1);
  this->mat_geo_gx_.setZero(4, 4);
  this->mat_geo_gy_.setZero(4, 4);
  this->mat_geo_gz_.setZero(4, 4);
  //
  // 5. Preenche as matrizes geométricas com G de Hermited
  //
  // Gx:
  // 2x2 superior esquerdo
  mat_geo_gx_(0, 0) = this->pt00_.x;
  mat_geo_gx_(0, 1) = this->pt01_.x;
  mat_geo_gx_(1, 0) = this->pt10_.x;
  mat_geo_gx_(1, 1) = this->pt11_.x;
  // 2x2 inferior esquerdo
  mat_geo_gx_(2, 0) = this->qu00_.x;
  mat_geo_gx_(2, 1) = this->qu01_.x;
  mat_geo_gx_(3, 0) = this->qu10_.x;
  mat_geo_gx_(3, 1) = this->qu11_.x;
  // 2x2 superior direito
  mat_geo_gx_(0, 2) = this->qv00_.x;
  mat_geo_gx_(0, 3) = this->qv01_.x;
  mat_geo_gx_(1, 2) = this->qv10_.x;
  mat_geo_gx_(1, 3) = this->qv11_.x;
  // 2x2 inferior direito
  mat_geo_gx_(2, 2) = this->tw00_.x;
  mat_geo_gx_(2, 3) = this->tw01_.x;
  mat_geo_gx_(3, 2) = this->tw10_.x;
  mat_geo_gx_(3, 3) = this->tw11_.x;
  //
  // Gy:
  // 2x2 superior esquerdo
  mat_geo_gy_(0, 0) = this->pt00_.y;
  mat_geo_gy_(0, 1) = this->pt01_.y;
  mat_geo_gy_(1, 0) = this->pt10_.y;
  mat_geo_gy_(1, 1) = this->pt11_.y;
  // 2x2 inferior esquerdo
  mat_geo_gy_(2, 0) = this->qu00_.y;
  mat_geo_gy_(2, 1) = this->qu01_.y;
  mat_geo_gy_(3, 0) = this->qu10_.y;
  mat_geo_gy_(3, 1) = this->qu11_.y;
  // 2x2 superior direito
  mat_geo_gy_(0, 2) = this->qv00_.y;
  mat_geo_gy_(0, 3) = this->qv01_.y;
  mat_geo_gy_(1, 2) = this->qv10_.y;
  mat_geo_gy_(1, 3) = this->qv11_.y;
  // 2x2 inferior direito
  mat_geo_gy_(2, 2) = this->tw00_.y;
  mat_geo_gy_(2, 3) = this->tw01_.y;
  mat_geo_gy_(3, 2) = this->tw10_.y;
  mat_geo_gy_(3, 3) = this->tw11_.y;
  //
  // Gz:
  // 2x2 superior esquerdo
  mat_geo_gz_(0, 0) = this->pt00_.z;
  mat_geo_gz_(0, 1) = this->pt01_.z;
  mat_geo_gz_(1, 0) = this->pt10_.z;
  mat_geo_gz_(1, 1) = this->pt11_.z;
  // 2x2 inferior esquerdo
  mat_geo_gz_(2, 0) = this->qu00_.z;
  mat_geo_gz_(2, 1) = this->qu01_.z;
  mat_geo_gz_(3, 0) = this->qu10_.z;
  mat_geo_gz_(3, 1) = this->qu11_.z;
  // 2x2 superior direito
  mat_geo_gz_(0, 2) = this->qv00_.z;
  mat_geo_gz_(0, 3) = this->qv01_.z;
  mat_geo_gz_(1, 2) = this->qv10_.z;
  mat_geo_gz_(1, 3) = this->qv11_.z;
  // 2x2 inferior direito
  mat_geo_gz_(2, 2) = this->tw00_.z;
  mat_geo_gz_(2, 3) = this->tw01_.z;
  mat_geo_gz_(3, 2) = this->tw10_.z;
  mat_geo_gz_(3, 3) = this->tw11_.z;
  //
  // 6. Iniciar matrix Hermite
  //
  this->mat_base_ = StartHermiteMatrix();
  //
  this->mat_geo_gx_ = this->GetH() * this->GetGx() * this->GetH().transpose();
  this->mat_geo_gy_ = this->GetH() * this->GetGy() * this->GetH().transpose();
  this->mat_geo_gz_ = this->GetH() * this->GetGz() * this->GetH().transpose();
}

// faz as multiplicações necessárias para 'Parameterize ( u, v )' e para as
// derivadas parciais
Ponto PatchHermite::CalculatePointUV() {
  Ponto point;
  // point = ( U * ( H * ( G * ( Ht * V ) ) ) )
  point.x = (this->GetU() * ((this->GetGx() * (this->GetV()))))(0, 0);
  point.y = (this->GetU() * ((this->GetGy() * (this->GetV()))))(0, 0);
  point.z = (this->GetU() * ((this->GetGz() * (this->GetV()))))(0, 0);

  if (std::isnan(point.x) || std::isnan(point.y) || std::isnan(point.z)) {
    cout << "-nan CalculatePointUV Hermite" << endl;
  }

  return point;
}

void PatchHermite::PrintAllMatrixPatchHermite() {
  cout << "H:\n" << mat_base_;
  cout << "U:\n" << mat_base_u_;
  cout << "V:\n" << mat_base_v_;

  cout << "Gx:\n" << mat_geo_gx_;
  cout << "Gy:\n" << mat_geo_gy_;
  cout << "Gz:\n" << mat_geo_gz_;
}

tuple<double, double> PatchHermite::FindUV(const Ponto& point) {
  unsigned int iMax = 0;

  // chute inicial
  double u_i = 0.5;
  double v_i = 0.5;
  ////////////////

  double delta_u = 0.0;  // o quanto o parâmetro terá de percorrer
  double delta_v = 0.0;  // o quanto o parâmetro terá de percorrer

  // Método de Jacobi para resolução de sistema
  Ponto point_i;
  // cout << "encontrar_u_v (" << p.id << "), usando Jacobi!";

  do {
    Vetor vector_tu = -(this->Qu(u_i, v_i));
    Vetor vector_tv = -(this->Qv(u_i, v_i));

    if (std::isnan(vector_tu.x) || std::isnan(vector_tu.y) ||
        std::isnan(vector_tu.z) || std::isnan(vector_tv.x) ||
        std::isnan(vector_tv.y) || std::isnan(vector_tv.z)) {
      cout << "-nan Tu e Tv" << endl;
    }

    point_i = this->Parameterize(u_i, v_i);  // palpite inicial

    if (std::isnan(point_i.x) || std::isnan(point_i.y) ||
        std::isnan(point_i.z)) {
      cout << "-nan p_i" << endl;
    }

    double A[3][3];
    // Matrix<double, 3,3> A;
    A[0][0] = vector_tu.x;
    A[0][1] = vector_tv.x;
    A[0][2] = point_i.x - point.x;
    A[1][0] = vector_tu.y;
    A[1][1] = vector_tv.y;
    A[1][2] = point_i.y - point.y;
    A[2][0] = vector_tu.z;
    A[2][1] = vector_tv.z;
    A[2][2] = point_i.z - point.z;

    int k = 0;
    double pivot = A[0][0];

    if (std::isnan(pivot)) {
      cout << "-nan pivo1" << endl;
    }

    //        if (fabs(pivo) < TOLERANCIA) {
    //            cout<<"pivo zero 1: "<<pivo<<endl;
    //        }

    while ((fabs(pivot) < TOLERANCIA) and (k < 2)) {
      ++k;
      pivot = A[k][0];
    }

    //  A.row(k).swap(A.row(0));

    for (int i = 0; i < 3; ++i) {
      double a = A[0][i];
      A[0][i] = A[k][i];
      A[k][i] = a;
    }

    if (fabs(pivot) < TOLERANCIA) {
      cout << "Erro! Não é possível encontrar as coordenadas paramétricas no "
              "ponto p"
           << point.id << " (" << point.x << ", " << point.y << ", " << point.z
           << ")" << endl;

      return make_tuple(-1.0, -1.0);
    }

    double A_10 = A[1][0];
    double A_20 = A[2][0];

    for (short j = 0; j < 3; ++j) {
      if (!(fabs(pivot) < TOLERANCIA)) {
        A[0][j] = static_cast<double>(A[0][j]) / pivot;
        A[1][j] = A[1][j] - A_10 * (A[0][j]);
        A[2][j] = A[2][j] - A_20 * (A[0][j]);
      } else {
        A[0][j] = 0.0;
        A[1][j] = A[1][j] - A_10 * (A[0][j]);
        A[2][j] = A[2][j] - A_20 * (A[0][j]);
      }
    }

    pivot = A[1][1];

    if (std::isnan(pivot)) {
      cout << "-nan pivo2" << endl;
    }

    if (fabs(pivot) < TOLERANCIA) {
      pivot = A[2][1];

      for (int i = 0; i < 3; ++i) {
        double a = A[1][i];
        A[1][i] = A[2][i];
        A[2][i] = a;
      }
    }

    double A_01 = A[0][1];
    double A_21 = A[0][1];

    for (short j = 0; j < 3; ++j) {
      if (!(fabs(pivot) < TOLERANCIA)) {
        A[1][j] = static_cast<double>(A[1][j]) / pivot;
        A[0][j] = A[0][j] - A_01 * (A[1][j]);
        A[2][j] = A[2][j] - A_21 * (A[1][j]);
      } else {
        A[1][j] = 0.0;
        A[0][j] = A[0][j] - A_01 * (A[1][j]);
        A[2][j] = A[2][j] - A_21 * (A[1][j]);
      }
    }

    delta_u = A[0][2];
    delta_v = A[1][2];

    u_i += delta_u;
    v_i += delta_v;

    if (++iMax > I_MAX) {
#if USE_PRINT_COMENT
      cout << "iMax alcançado!" << endl;
#endif  // #if USE_PRINT_COMENT
      break;
    }

    if (std::isnan(delta_u) || std::isnan(delta_v)) {
      cout << "-nan delta_u_v 1" << endl;
    }

    //        cout << "u = " << u_i << " " << "v = " << v_i << endl;
    //        cout << "delta_u = " << delta_u << " " << "delta_v = " << delta_v
    //        << endl;
  } while (fabs(delta_u) >= TOLERANCIA or fabs(delta_v) >= TOLERANCIA);
  // while ( p.distanciaPara(p_i) >= TOLERANCIA );

  if (std::isnan(delta_u) || std::isnan(delta_v)) {
    cout << "-nan delta_u_v 2" << endl;
  }

  if (u_i <= TOLERANCIA)
    u_i = 0.0;
  else if (u_i >= 1.0 - TOLERANCIA)
    u_i = 1.0;

  if (v_i <= TOLERANCIA)
    v_i = 0.0;
  else if (v_i >= 1.0 - TOLERANCIA)
    v_i = 1.0;

  // cout << "u = " << u_i << " " << "v = " << v_i << endl;

  return make_tuple(u_i, v_i);
}

// encontra as coordenadas 3D de um ponto p de parâmetros u, v
Ponto PatchHermite::Parameterize(double u, double v) {
  // cout << "Parameterize ( " << u << ", " << v << ")" << endl;
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
Vetor PatchHermite::Qu(double u, double v) {
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

  if (std::isnan(V.x) || std::isnan(V.y) || std::isnan(V.z)) {
    cout << "-nan V1" << endl;
  }

  return V;
}

// calcula o vetor tangente na direção v nas coordenadas paramétricas u, v
Vetor PatchHermite::Qv(double u, double v) {
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

  if (std::isnan(V.x) || std::isnan(V.y) || std::isnan(V.z)) {
    cout << "-nan V2" << endl;
  }
  return V;
}

// calcula a derivada parcial Quu nas coordenadas paramétricas u, v
Vetor PatchHermite::Quu(double u, double v) {
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

  if (std::isnan(V.x) || std::isnan(V.y) || std::isnan(V.z)) {
    cout << "-nan V3" << endl;
  }

  return V;
}

// calcula a derivada parcial Quv nas coordenadas paramétricas u, v
Vetor PatchHermite::Quv(double u, double v) {
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

  if (std::isnan(V.x) || std::isnan(V.y) || std::isnan(V.z)) {
    cout << "-nan V4" << endl;
  }
  return V;
}

// calcula a derivada parcial Qvu nas coordenadas paramétricas u, v
Vetor PatchHermite::Qvu(double u, double v) { return Quv(u, v); }

// calcula a derivada parcial Qvv nas coordenadas paramétricas u, v
Vetor PatchHermite::Qvv(double u, double v) {
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

  if (std::isnan(V.x) || std::isnan(V.y) || std::isnan(V.z)) {
    cout << "-nan V4" << endl;
  }
  return V;
}

// calcula o vetor tangente na direção u para o ponto p
Vetor PatchHermite::Qu(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Qu(get<0>(t), get<1>(t));
}

// calcula o vetor tangente na direção v para o ponto p
Vetor PatchHermite::Qv(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Qv(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Quu para o ponto p
Vetor PatchHermite::Quu(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Quu(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Quv para o ponto p
Vetor PatchHermite::Quv(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Quv(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Qvu para o ponto p
Vetor PatchHermite::Qvu(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Qvu(get<0>(t), get<1>(t));
}

// calcula a derivada parcial Qvv para o ponto p
Vetor PatchHermite::Qvv(const Ponto& point) {
  tuple<double, double> t = this->FindUV(point);
  return this->Qvv(get<0>(t), get<1>(t));
}

Matrix1x4 PatchHermite::GetU() const { return this->mat_base_u_; }

Matrix4x4 PatchHermite::GetGx() const { return this->mat_geo_gx_; }

Matrix4x4 PatchHermite::GetGy() const { return this->mat_geo_gy_; }

Matrix4x4 PatchHermite::GetGz() const { return this->mat_geo_gz_; }

Matrix4x1 PatchHermite::GetV() const { return this->mat_base_v_; }

Matrix4x4 PatchHermite::GetH() const { return this->mat_base_; }

Ponto PatchHermite::GetPt00() const { return this->pt00_; }

Ponto PatchHermite::GetPt01() const { return this->pt01_; }

Ponto PatchHermite::GetPt10() const { return this->pt10_; }

Ponto PatchHermite::GetPt11() const { return this->pt11_; }

Vetor PatchHermite::GetQu00() const { return this->qu00_; }

Vetor PatchHermite::GetQu01() const { return this->qu01_; }

Vetor PatchHermite::GetQu10() const { return this->qu10_; }

Vetor PatchHermite::GetQu11() const { return this->qu11_; }

Vetor PatchHermite::GetQv00() const { return this->qv00_; }

Vetor PatchHermite::GetQv01() const { return this->qv01_; }

Vetor PatchHermite::GetQv10() const { return this->qv10_; }

Vetor PatchHermite::GetQv11() const { return this->qv11_; }

Vetor PatchHermite::GetTw00() const { return this->tw00_; }

Vetor PatchHermite::GetTw01() const { return this->tw01_; }

Vetor PatchHermite::GetTw10() const { return this->tw10_; }

Vetor PatchHermite::GetTw11() const { return this->tw11_; }

Matrix4x4 PatchHermite::StartHermiteMatrix() {
  Matrix4x4 matrix_hermite;
  matrix_hermite(0, 0) = 2;
  matrix_hermite(0, 1) = -2;
  matrix_hermite(0, 2) = 1;
  matrix_hermite(0, 3) = 1;
  matrix_hermite(1, 0) = -3;
  matrix_hermite(1, 1) = 3;
  matrix_hermite(1, 2) = -2;
  matrix_hermite(1, 3) = -1;
  matrix_hermite(2, 0) = 0;
  matrix_hermite(2, 1) = 0;
  matrix_hermite(2, 2) = 1;
  matrix_hermite(2, 3) = 0;
  matrix_hermite(3, 0) = 1;
  matrix_hermite(3, 1) = 0;
  matrix_hermite(3, 2) = 0;
  matrix_hermite(3, 3) = 0;

  return matrix_hermite;
}
