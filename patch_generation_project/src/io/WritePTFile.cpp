#include "../../header/io/WritePTFile.h"

WritePTFile::WritePTFile() {}

void WritePTFile::writerPTFile(string _fileSave, VectorTuple48d vecTuple48d) {
  std::ofstream file;
  file.open(_fileSave);
  file << "#type BezierPatch (.pt) File" << std::endl;
  file << " " << std::endl;

  for (VectorTuple48d::iterator it = vecTuple48d.begin();
       it != vecTuple48d.end(); ++it) {
    file << "patch " << get<0>(*it) << std::endl;
    file << " " << std::endl;
    file << "pc00 " << get<1>(*it)(0, 0) << " " << get<1>(*it)(1, 0) << " "
         << get<1>(*it)(2, 0) << std::endl;
    file << "pc10 " << get<1>(*it)(3, 0) << " " << get<1>(*it)(4, 0) << " "
         << get<1>(*it)(5, 0) << std::endl;
    file << "pc20 " << get<1>(*it)(6, 0) << " " << get<1>(*it)(7, 0) << " "
         << get<1>(*it)(8, 0) << std::endl;
    file << "pc30 " << get<1>(*it)(9, 0) << " " << get<1>(*it)(10, 0) << " "
         << get<1>(*it)(11, 0) << std::endl;
    file << " " << std::endl;
    file << "pc01 " << get<1>(*it)(12, 0) << " " << get<1>(*it)(13, 0) << " "
         << get<1>(*it)(14, 0) << std::endl;
    file << "pc11 " << get<1>(*it)(15, 0) << " " << get<1>(*it)(16, 0) << " "
         << get<1>(*it)(17, 0) << std::endl;
    file << "pc21 " << get<1>(*it)(18, 0) << " " << get<1>(*it)(19, 0) << " "
         << get<1>(*it)(20, 0) << std::endl;
    file << "pc31 " << get<1>(*it)(21, 0) << " " << get<1>(*it)(22, 0) << " "
         << get<1>(*it)(23, 0) << std::endl;
    file << " " << std::endl;
    file << "pc02 " << get<1>(*it)(24, 0) << " " << get<1>(*it)(25, 0) << " "
         << get<1>(*it)(26, 0) << std::endl;
    file << "pc12 " << get<1>(*it)(27, 0) << " " << get<1>(*it)(28, 0) << " "
         << get<1>(*it)(29, 0) << std::endl;
    file << "pc22 " << get<1>(*it)(30, 0) << " " << get<1>(*it)(31, 0) << " "
         << get<1>(*it)(32, 0) << std::endl;
    file << "pc32 " << get<1>(*it)(33, 0) << " " << get<1>(*it)(34, 0) << " "
         << get<1>(*it)(35, 0) << std::endl;
    file << " " << std::endl;
    file << "pc03 " << get<1>(*it)(36, 0) << " " << get<1>(*it)(37, 0) << " "
         << get<1>(*it)(38, 0) << std::endl;
    file << "pc13 " << get<1>(*it)(39, 0) << " " << get<1>(*it)(40, 0) << " "
         << get<1>(*it)(41, 0) << std::endl;
    file << "pc23 " << get<1>(*it)(42, 0) << " " << get<1>(*it)(43, 0) << " "
         << get<1>(*it)(44, 0) << std::endl;
    file << "pc33 " << get<1>(*it)(45, 0) << " " << get<1>(*it)(46, 0) << " "
         << get<1>(*it)(47, 0) << std::endl;
    file << " " << std::endl;
  }

  file.close();
}

void WritePTFile::writeBPFile(string _fileSave, BP bp) {
  cout << "INICIO >>> ESCRITA" << endl;

  std::ofstream file;
  file.open(_fileSave);
  file << "File (.bp) generated for software genBezierPatches" << std::endl;
  file << "# of vertices " << get<0>(bp).size() << std::endl;
  file << "# of patches " << get<1>(bp).size() << std::endl;
  file << " " << std::endl;

  for (MapControlPoints::iterator it = get<0>(bp).begin();
       it != get<0>(bp).end(); ++it) {
    file << "v " /*<<get<0>(*it)<<" "*/ << get<1>(*it)(0) << " "
         << get<1>(*it)(1) << " " << get<1>(*it)(2) << std::endl;
  }

  for (PatchesControlPoints::iterator it = get<1>(bp).begin();
       it != get<1>(bp).end(); ++it) {
    file << "p " << (*it)(0, 0) << " " << (*it)(1, 0) << " " << (*it)(2, 0)
         << " " << (*it)(3, 0) << " " << (*it)(4, 0) << " " << (*it)(5, 0)
         << " " << (*it)(6, 0) << " " << (*it)(7, 0) << " " << (*it)(8, 0)
         << " " << (*it)(9, 0) << " " << (*it)(10, 0) << " " << (*it)(11, 0)
         << " " << (*it)(12, 0) << " " << (*it)(13, 0) << " " << (*it)(14, 0)
         << " " << (*it)(15, 0) << std::endl;
  }

  file.close();

  cout << "FIM >>> ESCRITA" << endl;
}

void WritePTFile::writeObjFile(string _fileSave, VectorTuplePoints vec) {
  std::ofstream file;
  file.open(_fileSave);
  file << "# File (.obj) generated for software genBezierPatches" << std::endl;
  file << "o teapot" << std::endl;
  file << " " << std::endl;

  for (VectorTuplePoints::iterator it = vec.begin(); it != vec.end(); ++it) {
    file << "v " /*<<get<0>(*it)<<" "*/ << get<2>(*it)(0) << " "
         << get<2>(*it)(1) << " " << get<2>(*it)(2) << std::endl;
  }

  file.close();
}
