#include "../../include/curvature/curvature_analytical.h"

extern double kTolerance;

// Dill, J. C. (1981). An Application of Color Graphics to the Display of
// Surface Curvature. Comp. Graph., Vol. 15, pag. 153-161. SIGGRAPH 81.

CurvatureAnalytical::CurvatureAnalytical(const PointAdaptive& v,
                                         PatchCoons& p) {
  tuple<double, double> t = p.FindUV(v);

  this->qu_ = p.Qu(get<0>(t), get<1>(t));
  this->qv_ = p.Qv(get<0>(t), get<1>(t));
  this->quu_ = p.Quu(get<0>(t), get<1>(t));
  this->quv_ = p.Quv(get<0>(t), get<1>(t));
  this->qvv_ = p.Qvv(get<0>(t), get<1>(t));
  // Vetor * Vetor -> produto vetorial
  this->prod_ = qu_ * qv_;

  //  Vetor ^ Vetor -> produto escalar
  this->a_ = prod_ ^ quu_;
  this->b_ = prod_ ^ quv_;
  this->c_ = prod_ ^ qvv_;

  // #pragma omp critical
  //     {
  //  if (std::isnan(get<0>(t)) || std::isnan(get<1>(t))) {
  //    cout << "-nan t" << endl;
  //  }
  //  cout << "==========================================================" <<
  //  endl; cout << "P - " << v.id << ": ( " << get<0>(t) << "," << get<1>(t) <<
  //  ")"
  //       << endl;
  //  cout << "Qu ( " << qu_.x_ << ", " << qu_.y_ << ", " << qu_.z_ << " )" <<
  //  endl; cout << "Qv ( " << qv_.x_ << ", " << qv_.y_ << ", " << qv_.z_ << "
  //  )" << endl; cout << "Quu ( " << quu_.x_ << ", " << quu_.y_ << ", " <<
  //  quu_.z_ << " )"
  //       << endl;
  //  cout << "Quv ( " << quv_.x_ << ", " << quv_.y_ << ", " << quv_.z_ << " )"
  //       << endl;
  //  cout << "Qvv ( " << qvv_.x_ << ", " << qvv_.y_ << ", " << qvv_.z_ << " )"
  //       << endl;
  //  //     }

  //	cout << "A = " << this->A << endl;
  //	cout << "B = " << this->B << endl;
  //	cout << "C = " << this->C << endl;
}

double CurvatureAnalytical::CalculateMeanCurvature() {
  // Vetor ^ Vetor -> produto escalar
  double prodModule = prod_.CalculateModule();

  if (prodModule <= kTolerance) return 0.0;  // regra de L'Hôpital

  double qvModule = qv_.CalculateModule();
  double quModule = qu_.CalculateModule();

  // H = ( A.|Qv|² - 2.B.Qu.Qv + C.|Qu|² ) / ( 2.| Qu x Qv |³ )
  auto resultado = (a_ * qvModule * qvModule - 2 * b_ * (qu_ ^ qv_) +
                    c_ * quModule * quModule) /
                   (2 * pow(prodModule, 3));

  // cout << "Ha = " << resultado << endl;
  // cout << "==========================================================" <<
  // endl;

  return (fabs(resultado) <= kTolerance) ? 0.0 : resultado;
}

double CurvatureAnalytical::CalculateGaussCurvature() {
  double prodModule = prod_.CalculateModule();

  if (prodModule <= kTolerance) return 0.0;  // regra de L'Hôpital

  // K = ( A.C - B² ) / | Qu x Qv |⁴
  double resultado =
      static_cast<double>(this->a_ * this->c_ - (this->b_ * this->b_)) /
      pow(prodModule, 4);

  // cout << "Ga = " << resultado << endl;

  return (fabs(resultado) <= kTolerance) ? 0.0 : resultado;
}
