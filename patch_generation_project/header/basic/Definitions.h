//
//  Definitions.h
//  genBezierPatches
//
//  Created by Tiago Guimarães Sombra on 12/11/15.
//  Copyright © 2015 TMeshSurf. All rights reserved.
//

#ifndef Definitions_h
#define Definitions_h

// Verificação de sistema operacional
#ifdef __APPLE__
#include <Eigen/Eigen>
#elif __linux__
#include "../../lib/eigen/Eigen/Eigen"
#else
#error "Unknown compiler"
#endif

#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace Eigen;

// tuple<id_patch, id_point, vector(x,y,z)>
typedef tuple<int, int, Vector3d> TuplePoint;
typedef vector<TuplePoint> VectorTuplePoints;

typedef pair<int, int> IdPatchPoint;
typedef tuple<int, Vector3d, vector<IdPatchPoint>> TuplePachtVerify;
typedef vector<TuplePachtVerify> VectorTuplePachtVerify;

typedef Matrix<double, 16, 16> Matrix16d;
typedef Matrix<double, 16, 1> Vector16d;
typedef Matrix<double, 48, 1> Vector48d;

typedef tuple<int, Vector48d> TupleVector48d;
typedef vector<TupleVector48d> VectorTuple48d;

typedef map<int, Vector3d> MapControlPoints;

typedef Matrix<int, 16, 1> PatchControlPoints;
typedef vector<PatchControlPoints> PatchesControlPoints;
typedef tuple<MapControlPoints, PatchesControlPoints> BP;

#endif /* Definitions_h */
