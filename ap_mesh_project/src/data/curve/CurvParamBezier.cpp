/* Classe que define uma curva paramétrica de Bezier
MDCC-UFC: Mestrado e Doutorado em Ciências da Computação
Universidade Federal do Ceará
Implementação da dissertação de Mestrado
Título: Geração Adaptativa de Malhas de Superfície com Controle de Curvatura
Autor: Daniel Márcio Batista de Siqueira
contato: siqueira@lia.ufc.br
Orientador: Creto Augusto Vidal
Co-Orientador: Joaquim Bento Cavalcante
This source code is under GNU General Public License v3 */

#include "../../../include/data/curve/CurvParamBezier.h"

Ponto CurvParamBezier::getP2() const { return this->P2; }

Ponto CurvParamBezier::getP3() const { return this->P3; }

void CurvParamBezier::setP2(const Ponto &p) { this->P2 = p; }

void CurvParamBezier::setP3(const Ponto &p) { this->P3 = p; }

CurvParamBezier::CurvParamBezier() : CurveAdaptiveParametric() {
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

  mat_geo_gx_(0, 0) = point_0_.x;
  mat_geo_gx_(1, 0) = point_1_.x;
  mat_geo_gx_(2, 0) = P2.x;
  mat_geo_gx_(3, 0) = P3.x;

  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, point_1_.y );
  //    Gy->setElement( 2, 0, P2.y );
  //    Gy->setElement( 3, 0, P3.y );

  mat_geo_gy_(0, 0) = point_0_.y;
  mat_geo_gy_(1, 0) = point_1_.y;
  mat_geo_gy_(2, 0) = P2.y;
  mat_geo_gy_(3, 0) = P3.y;

  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, point_1_.z );
  //    Gz->setElement( 2, 0, P2.z );
  //    Gz->setElement( 3, 0, P3.z );

  mat_geo_gz_(0, 0) = point_0_.z;
  mat_geo_gz_(1, 0) = point_1_.z;
  mat_geo_gz_(2, 0) = P2.z;
  mat_geo_gz_(3, 0) = P3.z;

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

CurvParamBezier::CurvParamBezier(Ponto p0, Ponto p1, Ponto p2, Ponto p3)
    : CurveAdaptiveParametric(p0, p1), P2(p2), P3(p3) {
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

  mat_geo_gx_(0, 0) = point_0_.x;
  mat_geo_gx_(1, 0) = point_1_.x;
  mat_geo_gx_(2, 0) = P2.x;
  mat_geo_gx_(3, 0) = P3.x;

  //    Gy->setElement( 0, 0, point_0_.y );
  //    Gy->setElement( 1, 0, point_1_.y );
  //    Gy->setElement( 2, 0, P2.y );
  //    Gy->setElement( 3, 0, P3.y );

  mat_geo_gy_(0, 0) = point_0_.y;
  mat_geo_gy_(1, 0) = point_1_.y;
  mat_geo_gy_(2, 0) = P2.y;
  mat_geo_gy_(3, 0) = P3.y;

  //    Gz->setElement( 0, 0, point_0_.z );
  //    Gz->setElement( 1, 0, point_1_.z );
  //    Gz->setElement( 2, 0, P2.z );
  //    Gz->setElement( 3, 0, P3.z );

  mat_geo_gz_(0, 0) = point_0_.z;
  mat_geo_gz_(1, 0) = point_1_.z;
  mat_geo_gz_(2, 0) = P2.z;
  mat_geo_gz_(3, 0) = P3.z;

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

  // 3. Calcula o comprimento da curva
  //
  CalculateLengthCurve();
  //
  ///////////////
}

CurvParamBezier::CurvParamBezier(CurvParamBezier *antiga)
    : CurveAdaptiveParametric(antiga) {
  this->P2 = antiga->P2;
  this->P3 = antiga->P3;
}

double CurvParamBezier::CalculateCurvature(double t) {
  // B'(t)= 3(1-t)²(point_1_-point_0_)+6t(1-t)(P2-point_1_)+3t²(P3-P2)
  double v0 = 3 * pow((1 - t), 2) * (point_1_.x - point_0_.x) +
              6 * t * (1 - t) * (P2.x - point_1_.x) +
              3 * pow(t, 2) * (P3.x - P2.x);
  double v1 = 3 * pow((1 - t), 2) * (point_1_.y - point_0_.y) +
              6 * t * (1 - t) * (P2.y - point_1_.y) +
              3 * pow(t, 2) * (P3.y - P2.y);
  double v2 = 3 * pow((1 - t), 2) * (point_1_.z - point_0_.z) +
              6 * t * (1 - t) * (P2.z - point_1_.z) +
              3 * pow(t, 2) * (P3.z - P2.z);

  // B''(t)= 6(1-t)(P2-2P1+point_0_)+6t(P3-2P2+point_1_)
  double v00 = 6 * (1 - t) * (P2.x - 2 * point_1_.x + point_0_.x) +
               6 * t * (P3.x - 2 * P2.x + point_1_.x);
  double v10 = 6 * (1 - t) * (P2.y - 2 * point_1_.y + point_0_.y) +
               6 * t * (P3.y - 2 * P2.y + point_1_.y);
  double v20 = 6 * (1 - t) * (P2.z - 2 * point_1_.z + point_0_.z) +
               6 * t * (P3.z - 2 * P2.z + point_1_.z);

  Vetor d1Vetor(v0, v1, v2);
  Vetor d2Vetor(v00, v10, v20);

  double k =
      ((d1Vetor.operator*(d2Vetor)).modulo()) / (pow(d1Vetor.modulo(), 3));

  return k;
}

CurvParamBezier::~CurvParamBezier() {}
