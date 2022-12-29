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
              CurveAdaptive* curve3, CurveAdaptive* curve4, Ponto pt11,
              Ponto pt21, Ponto pt12, Ponto pt22, bool signal_curve1 = true,
              bool signal_curve2 = true, bool signal_curve3 = true,
              bool signal_curve4 = true);

  PatchBezier(Ponto pt00, Ponto pt01, Ponto pt02, Ponto pt03, Ponto pt10,
              Ponto pt11, Ponto pt12, Ponto pt13, Ponto pt20, Ponto pt21,
              Ponto pt22, Ponto pt23, Ponto pt30, Ponto pt31, Ponto pt32,
              Ponto pt33, bool signal_curve1 = true, bool signal_curve2 = true,
              bool signal_curve3 = true, bool signal_curve4 = true);
  ~PatchBezier();

  Ponto CalculatePointUV();
  void PrintAllMatrixPatchBezier();
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
  Matrix4x4 GetB() const;

  Ponto GetPt00() const;
  Ponto GetPt01() const;
  Ponto GetPt02() const;
  Ponto GetPt03() const;

  Ponto GetPt10() const;
  Ponto GetPt11() const;
  Ponto GetPt12() const;
  Ponto GetPt13() const;

  Ponto GetPt20() const;
  Ponto GetPt21() const;
  Ponto GetPt22() const;
  Ponto GetPt23() const;

  Ponto GetPt30() const;
  Ponto GetPt31() const;
  Ponto GetPt32() const;
  Ponto GetPt33() const;

  void SetPt00(Ponto point);
  void SetPt01(Ponto point);
  void SetPt02(Ponto point);
  void SetPt03(Ponto point);

  void SetPt10(Ponto point);
  void SetPt11(Ponto point);
  void SetPt12(Ponto point);
  void SetPt13(Ponto point);

  void SetPt20(Ponto point);
  void SetPt21(Ponto point);
  void SetPt22(Ponto point);
  void SetPt23(Ponto point);

  void SetPt30(Ponto point);
  void SetPt31(Ponto point);
  void SetPt32(Ponto point);
  void SetPt33(Ponto point);

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
  Ponto pt03_;
  Ponto pt13_;
  Ponto pt23_;
  Ponto pt33_;
  Ponto pt02_;
  Ponto pt12_;
  Ponto pt22_;
  Ponto pt32_;
  Ponto pt01_;
  Ponto pt11_;
  Ponto pt21_;
  Ponto pt31_;
  Ponto pt00_;
  Ponto pt10_;
  Ponto pt20_;
  Ponto pt30_;

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
