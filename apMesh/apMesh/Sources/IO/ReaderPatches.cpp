#include "../../Headers/IO/ReaderPatches.h"

ReaderPatches::ReaderPatches()
{

}

std::list<BezierPatch *> ReaderPatches::loaderBezierPatchFile(string fileName)

{
    patch = new BezierPatch();

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
                        //int id_ponto;
                        //iss >> id_ponto;

                        //line = line.erase(0, 1);
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
                                patch->setPt00(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 1) {
                                patch->setPt10(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 2) {
                                patch->setPt20(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 3) {
                                patch->setPt30(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 4) {
                                patch->setPt01(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 5) {
                                patch->setPt11(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 6) {
                                patch->setPt21(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 7) {
                                patch->setPt31(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 8) {
                                patch->setPt02(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 9) {
                                patch->setPt12(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 10) {
                                patch->setPt22(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 11) {
                                patch->setPt32(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 12) {
                                patch->setPt03(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 13) {
                                patch->setPt13(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 14) {
                                patch->setPt23(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            } else if (i == 15) {
                                patch->setPt33(getPointVectorControlPoints(vectorControlPoints, p[i]));
                            }
                        }

                        p.erase(p.begin(), p.end());
                        patches.push_back(patch);
                        patch = new BezierPatch();
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

Ponto ReaderPatches::getPointVectorControlPoints(std::vector<Ponto> vectorPoints, unsigned long idPoint)
{
    for (std::vector<Ponto>::iterator it = vectorPoints.begin(); it != vectorPoints.end(); ++it) {
        if ((*it).getId() == idPoint) {
            return (*it);
        }
    }

    Ponto ponto;
    return ponto;
}

