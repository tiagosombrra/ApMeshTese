#include "../../Headers/Execution/MpiRun.h"

MpiRun::MpiRun()
{

}

MpiRun::~MpiRun()
{

}

int MpiRun::execute(int argc, char *argv[], Timer *timer)
{
#if USE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &SIZE_MPI);
  MPI_Comm_rank(MPI_COMM_WORLD, &RANK_MPI);
  MPI_Status status;

  malha = new Malha;
  geo = new Geometria();

  if (RANK_MPI == 0) {

      timer->endTimerParallel(0,0,0); // inicialização process 0

      // estimativa de caga para a distribuiçao nos patches
      estimateChargeofPatches(timer, argv[4]);

      timer->endTimerParallel(0,0,1); // Estimativa de carga process 0

      timer->initTimerParallel(0,0,8); // Overhead process 0

      // criação do vetor de curvas para a adapatção
      createVectorOfCurves();

      // distribuição dos patches entre os patches de acordo com suas cargas
      listBezierPt = orderPatchesDistribProcess(listBezierPt);


      //estimativa para cada processo
      double est0=0, est1=0, est2=0, est3=0, est4=0, est5=0, est6=0, est7=0;

      for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++)
        {
          switch ((*it)->getIdProcess()) {
            case 0:{
                est0 = est0 + (*it)->getNumberTriangle();
                break;
              }
            case 1:{
                est1 = est1 + (*it)->getNumberTriangle();
                break;
              }
            case 2:{
                est2 = est2 + (*it)->getNumberTriangle();
                break;
              }
            case 3:{
                est3 = est3 + (*it)->getNumberTriangle();
                break;
              }
            case 4:{
                est4 = est4 + (*it)->getNumberTriangle();
                break;
              }
            case 5:{
                est5 = est5 + (*it)->getNumberTriangle();
                break;
              }
            case 6:{
                est6 = est6 + (*it)->getNumberTriangle();
                break;
              }
            case 7:{
                est7 = est7 + (*it)->getNumberTriangle();
                break;
              }
            default:
              cout<<"ERRO>> Número de Processos"<<endl;
              break;
            }
        }

      cout<<"EST0 = "<<est0<<endl;
      cout<<"EST1 = "<<est1<<endl;
      cout<<"EST2 = "<<est2<<endl;
      cout<<"EST3 = "<<est3<<endl;
      cout<<"EST4 = "<<est4<<endl;
      cout<<"EST5 = "<<est5<<endl;
      cout<<"EST6 = "<<est6<<endl;
      cout<<"EST7 = "<<est7<<endl;

      double vecBezierPatchesOfProcess0[listBezierPt.size() * 48];
      double vecBezierPatches[listBezierPt.size() * 48];

      timer->endTimerParallel(0,0,8); // Overhead process 0

      int numberPatches = 0, i = 0, izero = 0;

      for (int j = 0; j < SIZE_MPI; j++) {

          timer->endTimerParallel(0,0,8); // Overhead process 0

          i = 0;

          for (std::list<BezierPatch*>::iterator it = getIteratorListPatches(numberPatches); it != listBezierPt.end();
               it++) {

              if (j != (*it)->getIdProcess()) {
                  break;
                } else {
                  numberPatches++;
                }

              vecBezierPatches[i] = (*it)->getPt00().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt00().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt00().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt10().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt10().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt10().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt20().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt20().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt20().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt30().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt30().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt30().z;
              i++;

              vecBezierPatches[i] = (*it)->getPt01().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt01().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt01().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt11().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt11().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt11().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt21().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt21().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt21().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt31().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt31().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt31().z;
              i++;

              vecBezierPatches[i] = (*it)->getPt02().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt02().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt02().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt12().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt12().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt12().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt22().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt22().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt22().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt32().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt32().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt32().z;
              i++;

              vecBezierPatches[i] = (*it)->getPt03().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt03().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt03().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt13().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt13().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt13().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt23().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt23().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt23().z;
              i++;
              vecBezierPatches[i] = (*it)->getPt33().x;
              i++;
              vecBezierPatches[i] = (*it)->getPt33().y;
              i++;
              vecBezierPatches[i] = (*it)->getPt33().z;
              i++;
            }

          if (j == 0) {
              izero = i;
              for (int m = 0; m < i; ++m) {
                  vecBezierPatchesOfProcess0[m] = vecBezierPatches[m];
                }

            }else {

              timer->endTimerParallel(0,0,8); // Overhead process 0

              timer->initTimerParallel(0,0,9); // Timer send and recv process 0

              MPI_Send(&i, 1, MPI_INT, j, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD);
              MPI_Send(&vecBezierPatches, i, MPI_DOUBLE, j, TAG_DOUBLE, MPI_COMM_WORLD);

              timer->endTimerParallel(0,0,9); // Timer send and recv process 0
            }
        }

      timer->endTimerParallel(0,0,8); // Overhead process 0

      GeradorAdaptativoPorCurvatura geradorAdaptativoPorCurvatura;
      geradorAdaptativoPorCurvatura.generatorMpi(vecBezierPatchesOfProcess0, izero, timer);

    } else {

      timer->endTimerParallel(0,0,0); // Inicialização process n

      timer->initTimerParallel(0,0,9); // Timer send and recv process n

      int sizePtBezierPatches = 0;
      MPI_Recv(&sizePtBezierPatches, 1, MPI_INT, 0, TAG_SIZE_OF_DOUBLE, MPI_COMM_WORLD, &status);

      double ptBezierPatches[sizePtBezierPatches];
      MPI_Recv(&ptBezierPatches, sizePtBezierPatches, MPI_DOUBLE, 0, TAG_DOUBLE, MPI_COMM_WORLD, &status);

      timer->endTimerParallel(0,0,9); // Timer send and recv process n

      GeradorAdaptativoPorCurvatura geradorAdaptativoPorCurvatura;
      geradorAdaptativoPorCurvatura.generatorMpi(ptBezierPatches, sizePtBezierPatches, timer);

    }

  //delete BezierPatch::B;
  //delete HermitePatch::H;
  delete timer;
  return MPI_Finalize();
#else
  return 0;
#endif // USE_MPI

}

void MpiRun::estimateChargeofPatches(Timer *timer, std::string entrada)
{
  ChargeEstimateProcess* cep = new ChargeEstimateProcess();
  listBezierPt = cep->chargeEstimateProcess(geo, timer, entrada);
  delete cep;
}

void MpiRun::createVectorOfCurves()
{
  Curva* curva;

  for (std::list<BezierPatch*>::iterator it = listBezierPt.begin(); it != listBezierPt.end(); it++) {

      if (curves.size() > 0) {
          if (verifyCurve((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30())) {
              curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
              curves.push_back(curva);
            }
          if (verifyCurve((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33())) {
              curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
              curves.push_back(curva);
            }
          if (verifyCurve((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33())) {
              curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
              curves.push_back(curva);
            }
          if (verifyCurve((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03())) {
              curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
              curves.push_back(curva);
            }
        } else if (curves.size() == 0) {
          curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt10(), (*it)->getPt20(), (*it)->getPt30());
          curves.push_back(curva);
          curva = new CurvParamBezier((*it)->getPt30(), (*it)->getPt31(), (*it)->getPt32(), (*it)->getPt33());
          curves.push_back(curva);
          curva = new CurvParamBezier((*it)->getPt03(), (*it)->getPt13(), (*it)->getPt23(), (*it)->getPt33());
          curves.push_back(curva);
          curva = new CurvParamBezier((*it)->getPt00(), (*it)->getPt01(), (*it)->getPt02(), (*it)->getPt03());
          curves.push_back(curva);
        }
    }
}

bool MpiRun::verifyCurve(Ponto p0, Ponto p1, Ponto p2, Ponto p3)
{
  for (vector<Curva*>::iterator it = this->curves.begin(); it != this->curves.end(); it++) {
      if (static_cast<CurvParamBezier*>(*it)
          ->getP0()
          .operator==(p0) and static_cast<CurvParamBezier*>(*it)
          ->getP1()
          .operator==(p1) and static_cast<CurvParamBezier*>(*it)
          ->getP2()
          .operator==(p2) and static_cast<CurvParamBezier*>(*it)
          ->getP3()
          .operator==(p3)) {
          return false;
        }
    }

  return true;
}

std::list<BezierPatch*> MpiRun::orderPatchesDistribProcess(std::list<BezierPatch*> listPatches)
{
#if USE_MPI
  std::vector<std::pair<double, int> > vectorEstimativeProcessPair;
  std::list<BezierPatch*> listPatchesOrder;

  if (SIZE_MPI > 1) {
      for (int i = 0; i < SIZE_MPI; i++) {
          vectorEstimativeProcessPair.push_back(std::make_pair(0.0, i));
        }

      for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {

          std::sort(vectorEstimativeProcessPair.begin(), vectorEstimativeProcessPair.end());

          //			for (std::vector<std::pair<double, int> >::iterator it =
          // vectorEstimativeProcessPair.begin();
          //         it != vectorEstimativeProcessPair.end();
          //         it++) {
          //			 std::cout<<"first: "<<(*it).first<<" second: "<<(*it).second<<std::endl;
          //    }
          (*it)->setIdProcess(vectorEstimativeProcessPair.front().second);
          // std::cout << "idProcess: " << vectorEstimativeProcessPair.front().second << std::endl;

          vectorEstimativeProcessPair.front().first += (*it)->getNumberTriangle();
          // std::cout << "front value: " << vectorEstimativeProcessPair.front().first << std::endl;
        }

      for (int i = 0; i < SIZE_MPI; i++) {
          for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {
              if ((*it)->getIdProcess() == i) {
                  listPatchesOrder.push_back((*it));
                }
            }
        }
      return listPatchesOrder;
    } else {
      for (std::list<BezierPatch*>::iterator it = listPatches.begin(); it != listPatches.end(); it++) {
          (*it)->setIdProcess(0);
        }
      return listPatches;
    }
#else
  return listPatches;
#endif //#if USE_OPENMPI
}

std::list<BezierPatch*>::iterator MpiRun::getIteratorListPatches(int numberPatches)
{
  std::list<BezierPatch*>::iterator it = listBezierPt.begin();
  advance(it, numberPatches);
  return it;
}
