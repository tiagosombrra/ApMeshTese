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
        std::vector<PointAdaptive> vectorControlPoints;
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

            PointAdaptive ponto(v[0], v[1], v[2], id_ponto);
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

PointAdaptive ReaderPatches::getPointVectorControlPoints(
    std::vector<PointAdaptive> vectorPoints, unsigned long idPoint) {
  for (std::vector<PointAdaptive>::iterator it = vectorPoints.begin();
       it != vectorPoints.end(); ++it) {
    if ((*it).GetId() == idPoint) {
      return (*it);
    }
  }

  PointAdaptive ponto;
  return ponto;
}

Geometry* ReaderPatches::readerPatches(Geometry* geo, string fileName) {
  ReaderPatches* pt = new ReaderPatches();
  std::list<PatchBezier*> listBezierPt = pt->loaderBezierPatchFile(fileName);
  delete pt;

  PointAdaptive* p00;
  PointAdaptive* p01;
  PointAdaptive* p02;
  PointAdaptive* p03;
  PointAdaptive* p10;
  PointAdaptive* p11;
  PointAdaptive* p12;
  PointAdaptive* p13;
  PointAdaptive* p20;
  PointAdaptive* p21;
  PointAdaptive* p22;
  PointAdaptive* p23;
  PointAdaptive* p30;
  PointAdaptive* p31;
  PointAdaptive* p32;
  PointAdaptive* p33;
  CurveAdaptive* patch_c1;
  CurveAdaptive* patch_c2;
  CurveAdaptive* patch_c3;
  CurveAdaptive* patch_c4;

  for (std::list<PatchBezier*>::iterator it = listBezierPt.begin();
       it != listBezierPt.end(); it++) {
    p00 = new VertexAdaptive((*it)->GetPt00().GetX(), (*it)->GetPt00().GetY(),
                             (*it)->GetPt00().GetZ());
    p00->SetId((*it)->GetPt00().GetId());
    p10 = new VertexAdaptive((*it)->GetPt10().GetX(), (*it)->GetPt10().GetY(),
                             (*it)->GetPt10().GetZ());
    p10->SetId((*it)->GetPt10().GetId());
    p20 = new VertexAdaptive((*it)->GetPt20().GetX(), (*it)->GetPt20().GetY(),
                             (*it)->GetPt20().GetZ());
    p20->SetId((*it)->GetPt20().GetId());
    p30 = new VertexAdaptive((*it)->GetPt30().GetX(), (*it)->GetPt30().GetY(),
                             (*it)->GetPt30().GetZ());
    p30->SetId((*it)->GetPt30().GetId());

    p01 = new VertexAdaptive((*it)->GetPt01().GetX(), (*it)->GetPt01().GetY(),
                             (*it)->GetPt01().GetZ());
    p01->SetId((*it)->GetPt01().GetId());
    p11 = new VertexAdaptive((*it)->GetPt11().GetX(), (*it)->GetPt11().GetY(),
                             (*it)->GetPt11().GetZ());
    p11->SetId((*it)->GetPt11().GetId());
    p21 = new VertexAdaptive((*it)->GetPt21().GetX(), (*it)->GetPt21().GetY(),
                             (*it)->GetPt21().GetZ());
    p21->SetId((*it)->GetPt21().GetId());
    p31 = new VertexAdaptive((*it)->GetPt31().GetX(), (*it)->GetPt31().GetY(),
                             (*it)->GetPt31().GetZ());
    p31->SetId((*it)->GetPt31().GetId());

    p02 = new VertexAdaptive((*it)->GetPt02().GetX(), (*it)->GetPt02().GetY(),
                             (*it)->GetPt02().GetZ());
    p02->SetId((*it)->GetPt02().GetId());
    p12 = new VertexAdaptive((*it)->GetPt12().GetX(), (*it)->GetPt12().GetY(),
                             (*it)->GetPt12().GetZ());
    p12->SetId((*it)->GetPt12().GetId());
    p22 = new VertexAdaptive((*it)->GetPt22().GetX(), (*it)->GetPt22().GetY(),
                             (*it)->GetPt22().GetZ());
    p22->SetId((*it)->GetPt22().GetId());
    p32 = new VertexAdaptive((*it)->GetPt32().GetX(), (*it)->GetPt32().GetY(),
                             (*it)->GetPt32().GetZ());
    p32->SetId((*it)->GetPt32().GetId());

    p03 = new VertexAdaptive((*it)->GetPt03().GetX(), (*it)->GetPt03().GetY(),
                             (*it)->GetPt03().GetZ());
    p03->SetId((*it)->GetPt03().GetId());
    p13 = new VertexAdaptive((*it)->GetPt13().GetX(), (*it)->GetPt13().GetY(),
                             (*it)->GetPt13().GetZ());
    p13->SetId((*it)->GetPt13().GetId());
    p23 = new VertexAdaptive((*it)->GetPt23().GetX(), (*it)->GetPt23().GetY(),
                             (*it)->GetPt23().GetZ());
    p23->SetId((*it)->GetPt23().GetId());
    p33 = new VertexAdaptive((*it)->GetPt33().GetX(), (*it)->GetPt33().GetY(),
                             (*it)->GetPt33().GetZ());
    p33->SetId((*it)->GetPt33().GetId());

    if (geo->VerifyCurveGeometry(p00, p10, p20, p30) == NULL) {
      patch_c1 = new CurveAdaptiveParametricBezier(*p00, *p10, *p20, *p30);
      geo->InsertCurve(patch_c1);
    } else {
      patch_c1 = geo->VerifyCurveGeometry(p00, p10, p20, p30);
    }

    if (geo->VerifyCurveGeometry(p30, p31, p32, p33) == NULL) {
      patch_c2 = new CurveAdaptiveParametricBezier(*p30, *p31, *p32, *p33);
      geo->InsertCurve(patch_c2);
    } else {
      patch_c2 = geo->VerifyCurveGeometry(p30, p31, p32, p33);
    }

    if (geo->VerifyCurveGeometry(p03, p13, p23, p33) == NULL) {
      patch_c3 = new CurveAdaptiveParametricBezier(*p03, *p13, *p23, *p33);
      geo->InsertCurve(patch_c3);
    } else {
      patch_c3 = geo->VerifyCurveGeometry(p03, p13, p23, p33);
    }

    if (geo->VerifyCurveGeometry(p00, p01, p02, p03) == NULL) {
      patch_c4 = new CurveAdaptiveParametricBezier(*p00, *p01, *p02, *p03);
      geo->InsertCurve(patch_c4);
    } else {
      patch_c4 = geo->VerifyCurveGeometry(p00, p01, p02, p03);
    }

    (*it) = new PatchBezier(patch_c1, patch_c2, patch_c3, patch_c4, *p11, *p21,
                            *p12, *p22);

    geo->InsertPatch((*it));
  }

  return geo;
}
