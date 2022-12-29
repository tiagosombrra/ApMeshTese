#include "../../include/input_output/ReaderPatches.h"

ReaderPatches::ReaderPatches() {}

std::list<PatchBezier*> ReaderPatches::loaderBezierPatchFile(string fileName)

{
  patch = new PatchBezier();

  if (!fileName.empty()) {
    std::ifstream fin(fileName);

    if (fin) {
      while (fin) {
        std::string line;
        std::vector<double> v;
        std::vector<Ponto> vectorControlPoints;
        std::vector<long> p;
        long id_ponto = 0;

        while (std::getline(fin, line)) {
          if (line[0] == 'v') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            // int id_ponto;
            // iss >> id_ponto;

            // line = line.erase(0, 1);
            double n;
            while (iss >> n) {
              v.push_back(n);
            }

            Ponto ponto(v[0], v[1], v[2], id_ponto);
            id_ponto++;
            vectorControlPoints.push_back(ponto);
            v.erase(v.begin(), v.end());
          }

          if (line[0] == 'p') {
            line = line.erase(0, 1);
            std::istringstream iss(line);
            long n;
            while (iss >> n) {
              p.push_back(n);
            }

            for (unsigned int i = 0; i < p.size(); i++) {
              if (i == 0) {
                patch->SetPt00(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 1) {
                patch->SetPt10(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 2) {
                patch->SetPt20(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 3) {
                patch->SetPt30(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 4) {
                patch->SetPt01(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 5) {
                patch->SetPt11(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 6) {
                patch->SetPt21(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 7) {
                patch->SetPt31(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 8) {
                patch->SetPt02(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 9) {
                patch->SetPt12(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 10) {
                patch->SetPt22(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 11) {
                patch->SetPt32(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 12) {
                patch->SetPt03(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 13) {
                patch->SetPt13(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 14) {
                patch->SetPt23(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              } else if (i == 15) {
                patch->SetPt33(
                    getPointVectorControlPoints(vectorControlPoints, p[i]));
              }
            }

            p.erase(p.begin(), p.end());
            patches.push_back(patch);
            patch = new PatchBezier();
          }
        }
      }
    } else {
      std::cout << "Error: file .bp is not readable." << std::endl;
    }

    fin.close();

  } else {
    std::cout << "Erro ao abrir o arquivo .bp" << std::endl;
  }

  return patches;
}

Ponto ReaderPatches::getPointVectorControlPoints(
    std::vector<Ponto> vectorPoints, unsigned long idPoint) {
  for (std::vector<Ponto>::iterator it = vectorPoints.begin();
       it != vectorPoints.end(); ++it) {
    if ((*it).getId() == idPoint) {
      return (*it);
    }
  }

  Ponto ponto;
  return ponto;
}

Geometry* ReaderPatches::readerPatches(Geometry* geo, string fileName) {
  ReaderPatches* pt = new ReaderPatches();
  std::list<PatchBezier*> listBezierPt = pt->loaderBezierPatchFile(fileName);
  delete pt;

  Ponto* p00;
  Ponto* p01;
  Ponto* p02;
  Ponto* p03;
  Ponto* p10;
  Ponto* p11;
  Ponto* p12;
  Ponto* p13;
  Ponto* p20;
  Ponto* p21;
  Ponto* p22;
  Ponto* p23;
  Ponto* p30;
  Ponto* p31;
  Ponto* p32;
  Ponto* p33;
  CurveAdaptive* patch_c1;
  CurveAdaptive* patch_c2;
  CurveAdaptive* patch_c3;
  CurveAdaptive* patch_c4;

  for (std::list<PatchBezier*>::iterator it = listBezierPt.begin();
       it != listBezierPt.end(); it++) {
    p00 =
        new Vertice((*it)->GetPt00().x, (*it)->GetPt00().y, (*it)->GetPt00().z);
    p00->setId((*it)->GetPt00().id);
    p10 =
        new Vertice((*it)->GetPt10().x, (*it)->GetPt10().y, (*it)->GetPt10().z);
    p10->setId((*it)->GetPt10().id);
    p20 =
        new Vertice((*it)->GetPt20().x, (*it)->GetPt20().y, (*it)->GetPt20().z);
    p20->setId((*it)->GetPt20().id);
    p30 =
        new Vertice((*it)->GetPt30().x, (*it)->GetPt30().y, (*it)->GetPt30().z);
    p30->setId((*it)->GetPt30().id);

    p01 =
        new Vertice((*it)->GetPt01().x, (*it)->GetPt01().y, (*it)->GetPt01().z);
    p01->setId((*it)->GetPt01().id);
    p11 =
        new Vertice((*it)->GetPt11().x, (*it)->GetPt11().y, (*it)->GetPt11().z);
    p11->setId((*it)->GetPt11().id);
    p21 =
        new Vertice((*it)->GetPt21().x, (*it)->GetPt21().y, (*it)->GetPt21().z);
    p21->setId((*it)->GetPt21().id);
    p31 =
        new Vertice((*it)->GetPt31().x, (*it)->GetPt31().y, (*it)->GetPt31().z);
    p31->setId((*it)->GetPt31().id);

    p02 =
        new Vertice((*it)->GetPt02().x, (*it)->GetPt02().y, (*it)->GetPt02().z);
    p02->setId((*it)->GetPt02().id);
    p12 =
        new Vertice((*it)->GetPt12().x, (*it)->GetPt12().y, (*it)->GetPt12().z);
    p12->setId((*it)->GetPt12().id);
    p22 =
        new Vertice((*it)->GetPt22().x, (*it)->GetPt22().y, (*it)->GetPt22().z);
    p22->setId((*it)->GetPt22().id);
    p32 =
        new Vertice((*it)->GetPt32().x, (*it)->GetPt32().y, (*it)->GetPt32().z);
    p32->setId((*it)->GetPt32().id);

    p03 =
        new Vertice((*it)->GetPt03().x, (*it)->GetPt03().y, (*it)->GetPt03().z);
    p03->setId((*it)->GetPt03().id);
    p13 =
        new Vertice((*it)->GetPt13().x, (*it)->GetPt13().y, (*it)->GetPt13().z);
    p13->setId((*it)->GetPt13().id);
    p23 =
        new Vertice((*it)->GetPt23().x, (*it)->GetPt23().y, (*it)->GetPt23().z);
    p23->setId((*it)->GetPt23().id);
    p33 =
        new Vertice((*it)->GetPt33().x, (*it)->GetPt33().y, (*it)->GetPt33().z);
    p33->setId((*it)->GetPt33().id);

    if (geo->verifyCurveGeometria(p00, p10, p20, p30) == NULL) {
      patch_c1 = new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
      geo->insereCurva(patch_c1);
    } else {
      patch_c1 = geo->verifyCurveGeometria(p00, p10, p20, p30);
    }

    if (geo->verifyCurveGeometria(p30, p31, p32, p33) == NULL) {
      patch_c2 = new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
      geo->insereCurva(patch_c2);
    } else {
      patch_c2 = geo->verifyCurveGeometria(p30, p31, p32, p33);
    }

    if (geo->verifyCurveGeometria(p03, p13, p23, p33) == NULL) {
      patch_c3 = new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
      geo->insereCurva(patch_c3);
    } else {
      patch_c3 = geo->verifyCurveGeometria(p03, p13, p23, p33);
    }

    if (geo->verifyCurveGeometria(p00, p01, p02, p03) == NULL) {
      patch_c4 = new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);
      geo->insereCurva(patch_c4);
    } else {
      patch_c4 = geo->verifyCurveGeometria(p00, p01, p02, p03);
    }

    (*it) = new PatchBezier(patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21,
                            *p12, *p22);

    geo->inserePatch((*it));
  }

  return geo;
}
