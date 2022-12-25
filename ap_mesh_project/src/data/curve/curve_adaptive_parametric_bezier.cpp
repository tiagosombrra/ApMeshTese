#include "../../../include/data/curve/curve_adaptive_parametric_bezier.h"

CurveAdaptiveParametricBezier::CurveAdaptiveParametricBezier()
    : CurveAdaptiveParametric() {
  // 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Bezier
  //
  //    M->setElement( 0, 0,-1 ); M->setElement( 0, 1, 3 ); M->setElement( 0,
  //    2,-3 ); M->setElement( 0, 3, 1 ); M->setElement( 1, 0, 3 );
  //    M->setElement( 1, 1,-6 ); M->setElement( 1, 2, 3 ); M->setElement( 1, 3,
  //    0 ); M->setElement( 2, 0,-3 ); M->setElement( 2, 1, 3 ); M->setElement(
  //    2, 2, 0 ); M->setElement( 2, 3, 0 ); M->setElement( 3, 0, 1 );
  //    M->setElement( 3, 1, 0 ); M->setElement( 3, 2, 0 ); M->setElement( 3, 3,
  //    0 );

  mat_base_(0, 0) = -1;
  mat_base_(0, 1) = 3;
  mat_base_(0, 2) = -3;
  mat_base_(0, 3) = 1;
  mat_base_(1, 0) = 3;
  mat_base_(1, 1) = -6;
  mat_base_(1, 2) = 3;
  mat_base_(1, 3) = 0;
  mat_base_(2, 0) = -3;
  mat_base_(2, 1) = 3;
  mat_base_(2, 2) = 0;
  mat_base_(2, 3) = 0;
  mat_base_(3, 0) = 1;
  mat_base_(3, 1) = 0;
  mat_base_(3, 2) = 0;
  mat_base_(3, 3) = 0;

  // 2. Preenche as matrizes geométricas com G de Bezier
  //
  //    Gx->setElement( 0, 0, point_0_.x );
  //    Gx->setElement( 1, 0, point_1_.x );
  //    Gx->setElement( 2, 0, P2.x );
  //    Gx->setElement( 3, 0, P3.x );

  mat_geo_gx_(0, 0) = point0_.x;
  mat_geo_gx_(1, 0) = point1_.x;
  mat_geo_gx_(2, 0) = point2_.x;
  mat_geo_gx_(3, 0) = point3_.x;

  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, point_1_.y );
  //    Gy->setElement( 2, 0, P2.y );
  //    Gy->setElement( 3, 0, P3.y );

  mat_geo_gy_(0, 0) = point0_.y;
  mat_geo_gy_(1, 0) = point1_.y;
  mat_geo_gy_(2, 0) = point2_.y;
  mat_geo_gy_(3, 0) = point3_.y;

  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, point_1_.z );
  //    Gz->setElement( 2, 0, P2.z );
  //    Gz->setElement( 3, 0, P3.z );

  mat_geo_gz_(0, 0) = point0_.z;
  mat_geo_gz_(1, 0) = point1_.z;
  mat_geo_gz_(2, 0) = point2_.z;
  mat_geo_gz_(3, 0) = point3_.z;

  Matrix4x1 m;
  m = this->GetMatBase() * this->GetMatGeoGx();
  // delete &this->Gx;
  this->SetMatGeoGx(m);

  m = this->GetMatBase() * this->GetMatGeoGy();
  // delete &this->Gx;
  this->SetMatGeoGy(m);

  m = this->GetMatBase() * this->GetMatGeoGz();
  // delete &this->Gx;
  this->SetMatGeoGz(m);

  // delete &M;

  // IMPORTANTE: Ao usar esse construtor, não esquecer de setar os pontos
  // inicial e final e os vetores tangentes inicial e final !!!
}

CurveAdaptiveParametricBezier::CurveAdaptiveParametricBezier(Ponto point0,
                                                             Ponto point1,
                                                             Ponto point2,
                                                             Ponto point3)
    : CurveAdaptiveParametric(point0, point1),
      point2_(point2),
      point3_(point3) {
  // 1. Preenche a matriz 'M' de 'CurvaParamétrica com a matriz de Bezier

  //    M->setElement( 0, 0,-1 ); M->setElement( 0, 1, 3 ); M->setElement( 0,
  //    2,-3 ); M->setElement( 0, 3, 1 ); M->setElement( 1, 0, 3 );
  //    M->setElement( 1, 1,-6 ); M->setElement( 1, 2, 3 ); M->setElement( 1, 3,
  //    0 ); M->setElement( 2, 0,-3 ); M->setElement( 2, 1, 3 ); M->setElement(
  //    2, 2, 0 ); M->setElement( 2, 3, 0 ); M->setElement( 3, 0, 1 );
  //    M->setElement( 3, 1, 0 ); M->setElement( 3, 2, 0 ); M->setElement( 3, 3,
  //    0 );

  mat_base_(0, 0) = -1;
  mat_base_(0, 1) = 3;
  mat_base_(0, 2) = -3;
  mat_base_(0, 3) = 1;
  mat_base_(1, 0) = 3;
  mat_base_(1, 1) = -6;
  mat_base_(1, 2) = 3;
  mat_base_(1, 3) = 0;
  mat_base_(2, 0) = -3;
  mat_base_(2, 1) = 3;
  mat_base_(2, 2) = 0;
  mat_base_(2, 3) = 0;
  mat_base_(3, 0) = 1;
  mat_base_(3, 1) = 0;
  mat_base_(3, 2) = 0;
  mat_base_(3, 3) = 0;

  // 2. Preenche as matrizes geométricas com G de Bezier

  //    Gx->setElement( 0, 0, point_0_.x );
  //    Gx->setElement( 1, 0, point_1_.x );
  //    Gx->setElement( 2, 0, P2.x );
  //    Gx->setElement( 3, 0, P3.x );

  mat_geo_gx_(0, 0) = point0_.x;
  mat_geo_gx_(1, 0) = point1_.x;
  mat_geo_gx_(2, 0) = point2.x;
  mat_geo_gx_(3, 0) = point3.x;

  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, point_1_.y );
  //    Gy->setElement( 2, 0, P2.y );
  //    Gy->setElement( 3, 0, P3.y );

  mat_geo_gy_(0, 0) = point0_.y;
  mat_geo_gy_(1, 0) = point1_.y;
  mat_geo_gy_(2, 0) = point2.y;
  mat_geo_gy_(3, 0) = point3.y;

  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, point_1_.z );
  //    Gz->setElement( 2, 0, P2.z );
  //    Gz->setElement( 3, 0, P3.z );

  mat_geo_gz_(0, 0) = point0_.z;
  mat_geo_gz_(1, 0) = point1_.z;
  mat_geo_gz_(2, 0) = point2.z;
  mat_geo_gz_(3, 0) = point3.z;

  Matrix4x1 mat_geo;
  mat_geo = this->GetMatBase() * this->GetMatGeoGx();
  // delete &this->Gx;
  this->SetMatGeoGx(mat_geo);

  mat_geo = this->GetMatBase() * this->GetMatGeoGy();
  // delete &this->Gx;
  this->SetMatGeoGy(mat_geo);

  mat_geo = this->GetMatBase() * this->GetMatGeoGz();
  // delete &this->Gx;
  this->SetMatGeoGz(mat_geo);

  // 3. Calcula o comprimento da curva
  //
  CalculateLengthCurve();
  //
  ///////////////
}

CurveAdaptiveParametricBezier::CurveAdaptiveParametricBezier(
    CurveAdaptiveParametricBezier *curve_adaptive_parametric_bezier)
    : CurveAdaptiveParametric(curve_adaptive_parametric_bezier) {
  this->point2_ = curve_adaptive_parametric_bezier->point2_;
  this->point3_ = curve_adaptive_parametric_bezier->point3_;
}

CurveAdaptiveParametricBezier::~CurveAdaptiveParametricBezier() {}

double CurveAdaptiveParametricBezier::CalculateCurvature(double t) {
  // B'(t)= 3(1-t)²(point_1_-point_0_)+6t(1-t)(P2-point_1_)+3t²(P3-P2)
  double v0 = 3 * pow((1 - t), 2) * (point1_.x - point0_.x) +
              6 * t * (1 - t) * (point2_.x - point1_.x) +
              3 * pow(t, 2) * (point3_.x - point2_.x);
  double v1 = 3 * pow((1 - t), 2) * (point1_.y - point0_.y) +
              6 * t * (1 - t) * (point2_.y - point1_.y) +
              3 * pow(t, 2) * (point3_.y - point2_.y);
  double v2 = 3 * pow((1 - t), 2) * (point1_.z - point0_.z) +
              6 * t * (1 - t) * (point2_.z - point1_.z) +
              3 * pow(t, 2) * (point3_.z - point2_.z);

  // B''(t)= 6(1-t)(P2-2P1+point_0_)+6t(P3-2P2+point_1_)
  double v00 = 6 * (1 - t) * (point2_.x - 2 * point1_.x + point0_.x) +
               6 * t * (point3_.x - 2 * point2_.x + point1_.x);
  double v10 = 6 * (1 - t) * (point2_.y - 2 * point1_.y + point0_.y) +
               6 * t * (point3_.y - 2 * point2_.y + point1_.y);
  double v20 = 6 * (1 - t) * (point2_.z - 2 * point1_.z + point0_.z) +
               6 * t * (point3_.z - 2 * point2_.z + point1_.z);

  Vetor vector_d1(v0, v1, v2);
  Vetor vector_d2(v00, v10, v20);

  double k = ((vector_d1.operator*(vector_d2)).modulo()) /
             (pow(vector_d1.modulo(), 3));

  return k;
}

Ponto CurveAdaptiveParametricBezier::GetPoint2() const { return this->point2_; }

Ponto CurveAdaptiveParametricBezier::GetPoint3() const { return this->point3_; }

void CurveAdaptiveParametricBezier::SetPoint2(const Ponto &p) {
  this->point2_ = p;
}

void CurveAdaptiveParametricBezier::SetPoint3(const Ponto &p) {
  this->point3_ = p;
}
