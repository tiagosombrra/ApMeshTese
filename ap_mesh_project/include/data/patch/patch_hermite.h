#ifndef DATA_PATCH_PATCH_HERMITE_H
#define DATA_PATCH_PATCH_HERMITE_H

#include "../../data/curve/curve_adaptive_parametric_hermite.h"
#include "patch_coons.h"

// As curvas devem ser definidas da esquerda para a direita, de baixo para
// cima em relação ao Patch !!!
class PatchHermite : public PatchCoons {
 public:
  PatchHermite();
  PatchHermite(PatchHermite* patch_hermite);
  // Ordem das curvas:
  //		C3
  //	C4		C2
  //		C1
  PatchHermite(CurveAdaptive* curve1, CurveAdaptive* curve2,
               CurveAdaptive* curve3, CurveAdaptive* curve4, Vetor tw00,
               Vetor tw10, Vetor tw01, Vetor tw11, bool signal_curve1 = true,
               bool signal_curve2 = true, bool signal_curve3 = true,
               bool signal_curve4 = true);
  ~PatchHermite() = default;

  Ponto CalculatePointUV();
  void PrintAllMatrixPatchHermite();
  // encontra as coordenadas parâmetricas u, v de um dado ponto p no patch,
  // ou a projeção desse ponto na superfície
  // tuple<double, double> encontrar_u_v(const Ponto& p);
  tuple<double, double> FindUV(const Ponto& point);

  // encontra o ponto p na curva dado um parâmetro p
  Ponto Parameterize(double u, double v);

  // calcula as derivadas parciais nas direçõe u e v
  Vetor Qu(double u, double v);
  Vetor Qv(double u, double v);
  Vetor Quu(double u, double v);
  Vetor Quv(double u, double v);
  Vetor Qvu(double u, double v);
  Vetor Qvv(double u, double v);

  // calcula as derivadas parciais de um ponto p
  Vetor Qu(const Ponto& point);
  Vetor Qv(const Ponto& point);
  Vetor Quu(const Ponto& point);
  Vetor Quv(const Ponto& point);
  Vetor Qvu(const Ponto& point);
  Vetor Qvv(const Ponto& point);

  Matrix1x4 GetU() const;
  Matrix4x4 GetGx() const;
  Matrix4x4 GetGy() const;
  Matrix4x4 GetGz() const;
  Matrix4x1 GetV() const;
  Matrix4x4 GetH() const;

  Ponto GetPt00() const;
  Ponto GetPt01() const;
  Ponto GetPt10() const;
  Ponto GetPt11() const;

  Vetor GetQu00() const;
  Vetor GetQu01() const;
  Vetor GetQu10() const;
  Vetor GetQu11() const;

  Vetor GetQv00() const;
  Vetor GetQv01() const;
  Vetor GetQv10() const;
  Vetor GetQv11() const;

  Vetor GetTw00() const;
  Vetor GetTw01() const;
  Vetor GetTw10() const;
  Vetor GetTw11() const;

 protected:
  Matrix4x4 StartHermiteMatrix();

  Ponto pt00_;
  Ponto pt01_;
  Vetor qv00_;
  Vetor qv01_;
  Ponto pt10_;
  Ponto pt11_;
  Vetor qv10_;
  Vetor qv11_;
  Vetor qu00_;
  Vetor qu01_;
  Vetor tw00_;
  Vetor tw01_;
  Vetor qu10_;
  Vetor qu11_;
  Vetor tw10_;
  Vetor tw11_;

  Matrix4x4 mat_geo_gx_;  // matriz geométrica para coordenada x
  Matrix4x4 mat_geo_gy_;  // matriz geométrica para coordenada y
  Matrix4x4 mat_geo_gz_;  // matriz geométrica para coordenada z

  Matrix4x4 mat_base_;    // matriz de Hermite
  Matrix4x1 mat_base_v_;  // matriz do parâmetro v
  Matrix1x4 mat_base_u_;  // matriz do parâmetro u

  bool signal_curve1_;
  bool signal_curve2_;
  bool signal_curve3_;
  bool signal_curve4_;
};
#endif  // DATA_PATCH_PATCH_HERMITE_H
