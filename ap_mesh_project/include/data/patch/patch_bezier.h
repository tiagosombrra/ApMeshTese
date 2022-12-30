#ifndef DATA_PATCH_PATCH_BEZIER_H
#define DATA_PATCH_PATCH_BEZIER_H

#include "../../data/curve/curve_adaptive_parametric_bezier.h"
#include "patch_coons.h"

// As curvas devem ser definidas da esquerda para a direita, de baixo para
// cima em relação ao Patch !!!

class PatchBezier : public PatchCoons {
 public:
  PatchBezier();
  PatchBezier(PatchBezier* patch_bezier);
  // Ordem das curvas:
  //		C3
  //	C4		C2
  //		C1
  PatchBezier(CurveAdaptive* curve1, CurveAdaptive* curve2,
              CurveAdaptive* curve3, CurveAdaptive* curve4, PointAdaptive pt11,
              PointAdaptive pt21, PointAdaptive pt12, PointAdaptive pt22,
              bool signal_curve1 = true, bool signal_curve2 = true,
              bool signal_curve3 = true, bool signal_curve4 = true);

  PatchBezier(PointAdaptive pt00, PointAdaptive pt01, PointAdaptive pt02,
              PointAdaptive pt03, PointAdaptive pt10, PointAdaptive pt11,
              PointAdaptive pt12, PointAdaptive pt13, PointAdaptive pt20,
              PointAdaptive pt21, PointAdaptive pt22, PointAdaptive pt23,
              PointAdaptive pt30, PointAdaptive pt31, PointAdaptive pt32,
              PointAdaptive pt33, bool signal_curve1 = true,
              bool signal_curve2 = true, bool signal_curve3 = true,
              bool signal_curve4 = true);
  ~PatchBezier();

  PointAdaptive CalculatePointUV();
  void PrintAllMatrixPatchBezier();
  // encontra as coordenadas parâmetricas u, v de um dado ponto p no patch,
  // ou a projeção desse ponto na superfície
  // tuple<double, double> encontrar_u_v(const Ponto& p);
  tuple<double, double> FindUV(const PointAdaptive& point);
  // encontra o ponto p na curva dado um parâmetro p
  PointAdaptive Parameterize(double u, double v);
  // calcula as derivadas parciais nas direçõe u e v
  Vetor Qu(double u, double v);
  Vetor Qv(double u, double v);
  Vetor Quu(double u, double v);
  Vetor Quv(double u, double v);
  Vetor Qvu(double u, double v);
  Vetor Qvv(double u, double v);

  // calcula as derivadas parciais de um ponto p
  Vetor Qu(const PointAdaptive& point);
  Vetor Qv(const PointAdaptive& point);
  Vetor Quu(const PointAdaptive& point);
  Vetor Quv(const PointAdaptive& point);
  Vetor Qvu(const PointAdaptive& point);
  Vetor Qvv(const PointAdaptive& point);

  Matrix1x4 GetU() const;
  Matrix4x4 GetGx() const;
  Matrix4x4 GetGy() const;
  Matrix4x4 GetGz() const;
  Matrix4x1 GetV() const;
  Matrix4x4 GetB() const;

  PointAdaptive GetPt00() const;
  PointAdaptive GetPt01() const;
  PointAdaptive GetPt02() const;
  PointAdaptive GetPt03() const;

  PointAdaptive GetPt10() const;
  PointAdaptive GetPt11() const;
  PointAdaptive GetPt12() const;
  PointAdaptive GetPt13() const;

  PointAdaptive GetPt20() const;
  PointAdaptive GetPt21() const;
  PointAdaptive GetPt22() const;
  PointAdaptive GetPt23() const;

  PointAdaptive GetPt30() const;
  PointAdaptive GetPt31() const;
  PointAdaptive GetPt32() const;
  PointAdaptive GetPt33() const;

  void SetPt00(PointAdaptive point);
  void SetPt01(PointAdaptive point);
  void SetPt02(PointAdaptive point);
  void SetPt03(PointAdaptive point);

  void SetPt10(PointAdaptive point);
  void SetPt11(PointAdaptive point);
  void SetPt12(PointAdaptive point);
  void SetPt13(PointAdaptive point);

  void SetPt20(PointAdaptive point);
  void SetPt21(PointAdaptive point);
  void SetPt22(PointAdaptive point);
  void SetPt23(PointAdaptive point);

  void SetPt30(PointAdaptive point);
  void SetPt31(PointAdaptive point);
  void SetPt32(PointAdaptive point);
  void SetPt33(PointAdaptive point);

  // gets and sets para estimativa de carga
  double getArea() const;
  void setArea(double value);

  double getKaMedio() const;
  void setKaMedio(double value);

  double getSegmentMedio() const;
  void setSegmentMedio(double value);

  double getAreaTriangle() const;
  void setAreaTriangle(double value);

  double getNumberTriangle() const;
  void setNumberTriangle(double value);

  int getIdProcess() const;
  void setIdProcess(int value);

  int getId_bezierPatch() const;
  void setId_bezierPatch(int value);

 protected:
  Matrix4x4 StartBezierMatrix();

  // Pontos de controle
  PointAdaptive pt03_;
  PointAdaptive pt13_;
  PointAdaptive pt23_;
  PointAdaptive pt33_;
  PointAdaptive pt02_;
  PointAdaptive pt12_;
  PointAdaptive pt22_;
  PointAdaptive pt32_;
  PointAdaptive pt01_;
  PointAdaptive pt11_;
  PointAdaptive pt21_;
  PointAdaptive pt31_;
  PointAdaptive pt00_;
  PointAdaptive pt10_;
  PointAdaptive pt20_;
  PointAdaptive pt30_;

  Matrix4x4 mat_geo_gx_;  // Matriz geométrica (x)
  Matrix4x4 mat_geo_gy_;  // Matriz geométrica (y)
  Matrix4x4 mat_geo_gz_;  // Matriz geométrica (z)

  Matrix4x4 mat_base_;    // matriz de Bezier
  Matrix1x4 mat_base_u_;  // matriz do parâmetro u
  Matrix4x1 mat_base_v_;  // matriz do parâmetro v

  bool signal_curve1_;
  bool signal_curve2_;
  bool signal_curve3_;
  bool signal_curve4_;

  // variaveis para estimativa de carga
  double area_;
  double ka_medio_;
  double segment_medio_;
  double area_triangle_;
  double number_triangle_;
  int id_process_;
  int id_patch_bezier_;
};
#endif  // DATA_PATCH_PATCH_BEZIER_H
