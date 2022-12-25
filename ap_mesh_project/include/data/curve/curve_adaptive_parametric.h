#ifndef DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_H
#define DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_H

#include <iostream>
#include <list>

#include "../../crab_mesh/numerical/bisection_equation_root.h"
#include "../../crab_mesh/numerical/equation_root_function.h"
#include "../../data/Definitions.h"
#include "../Vertice.h"
#include "../Vetor.h"
#include "curve_adaptive.h"

class CurveAdaptiveParametric : public CurveAdaptive {
 public:
  // IMPORTANTE: Ao usar o construtor default, não esquecer de setar
  // os pontos inicial e final e os vetores tangentes inicial e final !!!
  CurveAdaptiveParametric();
  // Esse construtor é mais recomendado !!! Evita erros ao esquecer de setar
  // os valores indicados na observação do construtor default !!!
  CurveAdaptiveParametric(Ponto point0, Ponto point1);
  CurveAdaptiveParametric(CurveAdaptiveParametric*);
  ~CurveAdaptiveParametric();

  // calcula o comprimento de curva de p1 a p2
  virtual double CalculateLengthPoints(const Ponto& point1,
                                       const Ponto& point2);
  // calcula o comprimento de curva até p
  virtual double CalculateLengthPoint(const Ponto& point);
  // calcula comprimento total "L" da curva
  virtual void CalculateLengthCurve();
  // calcula a curvatuta da curva
  virtual double CalculateCurvature(double);
  // calcula o comprimento de curva entre os parâmetros t1 e t2
  double CalculateParametricLength(double parameter1, double parameter2);
  // encontra o parâmetro t de um dado ponto p na curva, ou do ponto na
  // curva mais próximo de p
  double FindParameterByPoint(const Ponto& point);
  // encontra o ponto p na curva dado um parâmetro t
  Ponto FindPointByParameter(double parameter);
  // calcula o gradiente no parâmetro t
  Vetor CalculateGradientByParameter(double parameter);
  // retorna o ponto que fica na metade da curva
  Ponto CalculateMidpointByPoints(const Ponto& point1, const Ponto& point2);
  // retorna o ponto que fica na metade da curva
  double CalculateMidparameterByParamters(const double parameter1,
                                          const double parameter2);
  // ordena a lista de pontos de acordo com suas coordenadas paramétricas
  void SortPointsByParameters();
  void UpdateParameters(list<double> new_parameters);

  Matrix4x1 GetMatGeoGx() const;
  void SetMatGeoGx(Matrix4x1& mat_geo_gx);

  Matrix4x1 GetMatGeoGy() const;
  void SetMatGeoGy(Matrix4x1& mat_geo_gy);

  Matrix4x1 GetMatGeoGz() const;
  void SetMatGeoGz(Matrix4x1& mat_geo_gz);

  Matrix4x4 GetMatBase() const;
  void SetMatBase(Matrix4x4& mat_base);

  Matrix1x4 GetMatParameters() const;
  void SetMatParameters(Matrix1x4& mat_parameters);

  Ponto GetPoint0() const;
  void SetPoint0(const Ponto& point0);

  Ponto GetPoint1() const;
  void SetPoint1(const Ponto& point1);

  void SetPoint0Point1(const Ponto& point0, const Ponto& point1);

  // parâmetros dos pontos
  list<double> parameters_;

 protected:
  // retorna o ponto calculado por T x M x Gx,y,z
  Ponto CalculatePointT();
  double CalculateLength(double t1, double t2, int parts, int points);
  double CalculateMidpointBisection(double t1, double t2);

  // ponto inicial
  Ponto point_0_;
  // ponto final
  Ponto point_1_;
  // matrizes geométricas
  Matrix4x1 mat_geo_gx_;
  Matrix4x1 mat_geo_gy_;
  Matrix4x1 mat_geo_gz_;
  // matriz característica da curva
  Matrix4x4 mat_base_;
  // matriz dos parâmetros
  Matrix1x4 mat_parameters_;
};
#endif  // DATA_CURVE_CURVE_ADAPTIVE_PARAMETRIC_H
