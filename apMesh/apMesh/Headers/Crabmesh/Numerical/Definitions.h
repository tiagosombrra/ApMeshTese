#ifndef _NUMERICAL_DEFINITIONS_H_
#define _NUMERICAL_DEFINITIONS_H_

#include "../../Data/Definitions.h"

#if USE_NUMERICAL

//classes for NumericalMethods library
namespace Numerical
{
    using Data::Short;
	using Data::UShort;
	using Data::Int;
	using Data::LInt;
	using Data::UInt;
	using Data::ULInt;
	using Data::Real;
	using Data::LReal;

    class OrderedPoints;

    template <Data::UInt n> class MultiVariableFunction;
    class Function;
#if USE_EXPRTK
    class InterpretedFunction;
#endif //#if USE_EXPRTK
    class IPolynomial;
    template <Data::UInt deg> class TPolynomial;
    class Polynomial;

    class InterpolationFunction;

    class ApproximationFunctions;

    class DerivativePolynomial;

    class IMatrix;
    template <typename Type, UInt rows, UInt cols> class TMatrix;
    template <typename Type> class TDMatrix;
    class IDenseMatrix;
    template <UInt rows, UInt cols> class TDenseMatrix;
    class DenseMatrix;
    class ISparseMatrix;
    template <UInt rows, UInt cols, UInt cap> class TSparseMatrix;
    class SparseMatrix;
    class ITriangularMatrix;
    class ILowerTriangularMatrix;
    template <UInt rows> class TLowerTriangularMatrix;
    class LowerTriangularMatrix;
    class IUpperTriangularMatrix;
    template <UInt rows> class TUpperTriangularMatrix;
    class UpperTriangularMatrix;
    class IBandMatrix;
    template <UInt rows, UInt lowerBand, UInt upperBand> class TBandMatrix;
    class BandMatrix;
    class TridiagonalMatrix;
    template <UInt rows> class TDiagonalMatrix;
    class DiagonalMatrix;
    class ISymmetricMatrix;
    template <UInt rows> class TSymmetricMatrix;
    class SymmetricMatrix;
    class IColumnMatrix;
    template <UInt rows> class TColumnMatrix;
    class ColumnMatrix;
    class IRowMatrix;
    template <UInt cols> class TRowMatrix;
    class RowMatrix;
    class IPermutationMatrix;
    template <UInt rows> class TPermutationMatrix;
    class PermutationMatrix;
    class IdentityMatrix;
    class ZeroMatrix;
    class ILUMatrix;
    template <UInt rows> class TLUMatrix;
    class LUMatrix;
    class LMatrix;
    class UMatrix;
    class TransformationMatrix;
    class GeneralTransformationMatrix;
    class TranslationMatrix;
    class ScaleMatrix;
    class RotationMatrix;
    class ShearMatrix;
    class ReflectionMatrix;
    class BasisChangeMatrix;
    class HouseholderMatrix;
    class JacobiRotationMatrix;

    class ODESystem;

    class ChebyshevPoints;

    class EquationRootFinder;
    class BisectionEquationRootFinder;
    class ClosestBisectionEquationRootFinder;
    class MaxBisectionEquationRootFinder;
    class MinBisectionEquationRootFinder;
    class MaxMinBisectionEquationRootFinder;
    class FalsePositionEquationRootFinder;
    class ClosestFalsePositionEquationRootFinder;
    class MaxFalsePositionEquationRootFinder;
    class MinFalsePositionEquationRootFinder;
    class MaxMinFalsePositionEquationRootFinder;
    class FixedPointEquationRootFinder;
    class NewtonRaphsonEquationRootFinder;
    class SteffensenEquationRootFinder;
    class SecantEquationRootFinder;

    class DerivativeCalculator;
    class NewtonDerivativeCalculator;
    class ForwardNewtonDerivativeCalculator;
    class BackwardNewtonDerivativeCalculator;
    class CentralNewtonDerivativeCalculator;
    class CombinedNewtonDerivativeCalculator;

    class IntegralCalculator;
    class PointBasedIntegralCalculator;
    class FunctionBasedIntegralCalculator;
    class NewtonCotesIntegralCalculator;
    class ClosedNewtonCotesIntegralCalculator;
    class OpenNewtonCotesIntegralCalculator;
    class GaussLegendreIntegralCalculator;
    class RombergIntegralCalculator;

    class LinearSystemSolver;
    class GaussianEliminationLinearSystemSolver;
    class LUDecompositionLinearSystemSolver;
    class CholeskyFatorationLinearSystemSolver;
    class FixedPointLinearSystemSolver;
    class GaussJacobiLinearSystemSolver;
    class GaussSeidelLinearSystemSolver;

    class Interpolator;
    class LagrangeInterpolator;
    class NewtonInterpolator;

    class Approximator;
    class LeastSquaresApproximator;

    class ODESolver;
    class SingleStepODESolver;
    class MultiStepODESolver;
    class RungeKuttaODESolver;
    class ExplicitODESolver;
    class AdamsBashforthODESolver;
    class ImplicitODESolver;
    class AdamsMoultonODESolver;
    class PredictorCorrectorODESolver;

    class MatrixTridiagonalizer;
    class HouseholderMatrixTridiagonalizer;

    class EigenvalueFinder;
    class EigenvectorFinder;

    class EigensystemSolver;
    class HouseholderEigensystemSolver;
    class JacobiEigensystemSolver;
    class QREigensystemSolver;
    class PowerEigensystemSolver;
    class RegularPowerEigensystemSolver;
    class InversePowerEigensystemSolver;
    class ShiftedPowerEigensystemSolver;
}

#endif //#if USE_NUMERICAL

#endif //#ifndef _NUMERICAL_DEFINITIONS_H_
