#include "../../../include/data/curve/curve_adaptive_parametric_hermite.h"

CurveAdaptiveParametricHermite::CurveAdaptiveParametricHermite()
    : CurveAdaptiveParametric() {
  // 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Hermite
  //
  //    M->setElement( 0, 0, 2 ); M->setElement( 0, 1,-2 ); M->setElement( 0, 2,
  //    1 ); M->setElement( 0, 3, 1 ); M->setElement( 1, 0,-3 ); M->setElement(
  //    1, 1, 3 ); M->setElement( 1, 2,-2 ); M->setElement( 1, 3,-1 );
  //    M->setElement( 2, 0, 0 ); M->setElement( 2, 1, 0 ); M->setElement( 2, 2,
  //    1 ); M->setElement( 2, 3, 0 ); M->setElement( 3, 0, 1 ); M->setElement(
  //    3, 1, 0 ); M->setElement( 3, 2, 0 ); M->setElement( 3, 3, 0 );
  mat_base_(0, 0) = 2;
  mat_base_(0, 1) = -2;
  mat_base_(0, 2) = 1;
  mat_base_(0, 3) = 1;
  mat_base_(1, 0) = -3;
  mat_base_(1, 1) = 3;
  mat_base_(1, 2) = -2;
  mat_base_(1, 3) = -1;
  mat_base_(2, 0) = 0;
  mat_base_(2, 1) = 0;
  mat_base_(2, 2) = 1;
  mat_base_(2, 3) = 0;
  mat_base_(3, 0) = 1;
  mat_base_(3, 1) = 0;
  mat_base_(3, 2) = 0;
  mat_base_(3, 3) = 0;
  //
  ////////////////////////////////////////////////////////////////////////////

  // 2. Preenche as matrizes geométricas com G de Hermite
  //
  //    Gx->setElement( 0, 0, point_0_.x );
  //    Gx->setElement( 1, 0, P1.x );
  //    Gx->setElement( 2, 0,DP0.x );
  //    Gx->setElement( 3, 0,DP1.x );
  mat_geo_gx_(0, 0) = point0_.GetX();
  mat_geo_gx_(1, 0) = point1_.GetX();
  mat_geo_gx_(2, 0) = vector0_.x_;
  mat_geo_gx_(3, 0) = vector1_.x_;
  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, P1.y );
  //    Gy->setElement( 2, 0,DP0.y );
  //    Gy->setElement( 3, 0,DP1.y );
  mat_geo_gy_(0, 0) = point0_.GetY();
  mat_geo_gy_(1, 0) = point1_.GetY();
  mat_geo_gy_(2, 0) = vector0_.y;
  mat_geo_gy_(3, 0) = vector1_.y;
  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, P1.z );
  //    Gz->setElement( 2, 0,DP0.z );
  //    Gz->setElement( 3, 0,DP1.z );
  mat_geo_gz_(0, 0) = point0_.GetZ();
  mat_geo_gz_(1, 0) = point1_.GetZ();
  mat_geo_gz_(2, 0) = vector0_.z;
  mat_geo_gz_(3, 0) = vector1_.z;
  //

  this->mat_geo_gx_ = this->GetMatBase() * this->GetMatGeoGx();
  this->mat_geo_gy_ = this->GetMatBase() * this->GetMatGeoGy();
  this->mat_geo_gz_ = this->GetMatBase() * this->GetMatGeoGz();

  delete &mat_base_;

  // IMPORTANTE: Ao usar esse construtor, não esquecer de setar os pontos
  // inicial e final e os vetores tangentes inicial e final !!!
}

CurveAdaptiveParametricHermite::CurveAdaptiveParametricHermite(
    PointAdaptive point0, PointAdaptive point1, Vetor vector0, Vetor vector1)
    : CurveAdaptiveParametric(point0, point1),
      vector0_(vector0),
      vector1_(vector1) {
  // 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Hermite
  //
  //    M->setElement( 0, 0, 2 ); M->setElement( 0, 1,-2 ); M->setElement( 0, 2,
  //    1 ); M->setElement( 0, 3, 1 ); M->setElement( 1, 0,-3 ); M->setElement(
  //    1, 1, 3 ); M->setElement( 1, 2,-2 ); M->setElement( 1, 3,-1 );
  //    M->setElement( 2, 0, 0 ); M->setElement( 2, 1, 0 ); M->setElement( 2, 2,
  //    1 ); M->setElement( 2, 3, 0 ); M->setElement( 3, 0, 1 ); M->setElement(
  //    3, 1, 0 ); M->setElement( 3, 2, 0 ); M->setElement( 3, 3, 0 );
  mat_base_(0, 0) = 2;
  mat_base_(0, 1) = -2;
  mat_base_(0, 2) = 1;
  mat_base_(0, 3) = 1;
  mat_base_(1, 0) = -3;
  mat_base_(1, 1) = 3;
  mat_base_(1, 2) = -2;
  mat_base_(1, 3) = -1;
  mat_base_(2, 0) = 0;
  mat_base_(2, 1) = 0;
  mat_base_(2, 2) = 1;
  mat_base_(2, 3) = 0;
  mat_base_(3, 0) = 1;
  mat_base_(3, 1) = 0;
  mat_base_(3, 2) = 0;
  mat_base_(3, 3) = 0;
  //
  // 2. Preenche as matrizes geométricas com G de Hermite
  //
  //    Gx->setElement( 0, 0, point_0_.x );
  //    Gx->setElement( 1, 0, P1.x );
  //    Gx->setElement( 2, 0,DP0.x );
  //    Gx->setElement( 3, 0,DP1.x );
  mat_geo_gx_(0, 0) = point0_.GetX();
  mat_geo_gx_(1, 0) = point1_.GetX();
  mat_geo_gx_(2, 0) = vector0_.x_;
  mat_geo_gx_(3, 0) = vector1_.x_;
  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, P1.y );
  //    Gy->setElement( 2, 0,DP0.y );
  //    Gy->setElement( 3, 0,DP1.y );
  mat_geo_gy_(0, 0) = point0_.GetY();
  mat_geo_gy_(1, 0) = point1_.GetY();
  mat_geo_gy_(2, 0) = vector0_.y;
  mat_geo_gy_(3, 0) = vector1_.y;
  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, P1.z );
  //    Gz->setElement( 2, 0,DP0.z );
  //    Gz->setElement( 3, 0,DP1.z );
  mat_geo_gz_(0, 0) = point0_.GetZ();
  mat_geo_gz_(1, 0) = point1_.GetZ();
  mat_geo_gz_(2, 0) = vector0_.z;
  mat_geo_gz_(3, 0) = vector1_.z;
  //

  this->mat_geo_gx_ = this->GetMatBase() * this->GetMatGeoGx();
  this->mat_geo_gy_ = this->GetMatBase() * this->GetMatGeoGy();
  this->mat_geo_gz_ = this->GetMatBase() * this->GetMatGeoGz();

  // 3. Calcula o comprimento da curva
  //
  CalculateLengthCurve();
  //
  ///////////////
}

CurveAdaptiveParametricHermite::CurveAdaptiveParametricHermite(
    CurveAdaptiveParametricHermite *curve_adaptive_parametric_hermite)
    : CurveAdaptiveParametric(curve_adaptive_parametric_hermite) {
  this->vector0_ = curve_adaptive_parametric_hermite->vector0_;
  this->vector1_ = curve_adaptive_parametric_hermite->vector1_;
}

CurveAdaptiveParametricHermite::~CurveAdaptiveParametricHermite() {}

double CurveAdaptiveParametricHermite::CalculateCurvature(double t) {
  // B'(t)= 3(1-t)²(P1-point_0_)+6t(1-t)(P2-P1)+3t²(P3-P2)
  double v0 = 3 * pow((1 - t), 2) * (vector0_.x_ - point0_.GetX()) +
              6 * t * (1 - t) * (vector1_.x_ - vector0_.x_) +
              3 * pow(t, 2) * (point1_.GetX() - vector1_.x_);
  double v1 = 3 * pow((1 - t), 2) * (vector0_.y - point0_.GetY()) +
              6 * t * (1 - t) * (vector1_.y - vector0_.y) +
              3 * pow(t, 2) * (point1_.GetY() - vector1_.y);
  double v2 = 3 * pow((1 - t), 2) * (point1_.GetZ() - point0_.GetZ()) +
              6 * t * (1 - t) * (vector1_.z - vector0_.z) +
              3 * pow(t, 2) * (point1_.GetZ() - vector1_.z);

  // B''(t)= 6(1-t)(P2-2P1+point_0_)+6t(P3-2P2+P1)
  double v00 = 6 * (1 - t) * (vector1_.x_ - 2 * vector0_.x_ + point0_.GetX()) +
               6 * t * (point1_.GetX() - 2 * vector1_.x_ + vector0_.x_);
  double v10 = 6 * (1 - t) * (vector1_.y - 2 * vector0_.y + point0_.GetY()) +
               6 * t * (point1_.GetY() - 2 * vector1_.y + vector0_.y);
  double v20 = 6 * (1 - t) * (vector1_.z - 2 * vector0_.z + point0_.GetZ()) +
               6 * t * (point1_.GetZ() - 2 * vector1_.z + vector0_.z);

  Vetor vector_d1(v0, v1, v2);
  Vetor vector_d2(v00, v10, v20);

  double k = ((vector_d1.operator*(vector_d2)).modulo()) /
             (pow(vector_d1.modulo(), 3));

  return k;
}

Vetor CurveAdaptiveParametricHermite::GetVector0() const {
  return this->vector0_;
}

void CurveAdaptiveParametricHermite::SetVector0(const Vetor &vector0) {
  this->vector0_ = vector0;
}

Vetor CurveAdaptiveParametricHermite::GetVector1() const {
  return this->vector1_;
}

void CurveAdaptiveParametricHermite::SetVector1(const Vetor &vector1) {
  this->vector1_ = vector1;
}
