#include "../../include/input_output/write_obj_file.h"

#include <memory>

WriteOBJFile::WriteOBJFile() {}

WriteOBJFile::~WriteOBJFile() {}

void WriteOBJFile::WriteCurvaturePatches(std::vector<double> patches,
                                         double max_value) {
  stringstream name_file;
  name_file << NAME_MODEL + "_analise_curvature_patches.log";

  ofstream file(name_file.str().c_str());

  file << "File Analise Curvature" << endl << endl;

  std::vector<double> vec_0_10;
  std::vector<double> vec_10_20;
  std::vector<double> vec_20_30;
  std::vector<double> vec_30_40;
  std::vector<double> vec_40_50;
  std::vector<double> vec_50_60;
  std::vector<double> vec_60_70;
  std::vector<double> vec_70_80;
  std::vector<double> vec_80_90;
  std::vector<double> vec_90_100;

  for (std::vector<double>::iterator it = patches.begin(); it != patches.end();
       it++) {
    double value;
    value = (*it) / max_value;
    if (0.0 <= value && value <= 0.1) {
      vec_0_10.push_back(value);
    } else if (0.1 < value && value <= 0.2) {
      vec_10_20.push_back(value);
    } else if (0.2 < value && value <= 0.3) {
      vec_20_30.push_back(value);
    } else if (0.3 < value && value <= 0.4) {
      vec_30_40.push_back(value);
    } else if (0.4 < value && value <= 0.5) {
      vec_40_50.push_back(value);
    } else if (0.5 < value && value <= 0.6) {
      vec_50_60.push_back(value);
    } else if (0.6 < value && value <= 0.7) {
      vec_60_70.push_back(value);
    } else if (0.7 < value && value <= 0.8) {
      vec_70_80.push_back(value);
    } else if (0.8 < value && value <= 0.9) {
      vec_80_90.push_back(value);
    } else if (0.9 < value && value <= 1) {
      vec_90_100.push_back(value);
    }
  }

  file << vec_0_10.size() / static_cast<double>(patches.size()) << endl;
  file << vec_10_20.size() / static_cast<double>(patches.size()) << endl;
  file << vec_20_30.size() / static_cast<double>(patches.size()) << endl;
  file << vec_30_40.size() / static_cast<double>(patches.size()) << endl;
  file << vec_40_50.size() / static_cast<double>(patches.size()) << endl;
  file << vec_50_60.size() / static_cast<double>(patches.size()) << endl;
  file << vec_60_70.size() / static_cast<double>(patches.size()) << endl;
  file << vec_70_80.size() / static_cast<double>(patches.size()) << endl;
  file << vec_80_90.size() / static_cast<double>(patches.size()) << endl;
  file << vec_90_100.size() / static_cast<double>(patches.size()) << endl;

  file.flush();

  file.close();
}

bool WriteOBJFile::WriteMeshOBJFile(std::shared_ptr<MeshAdaptive> mesh,
                                    unsigned int step, int process) {
  std::stringstream name_file;
  name_file << "mesh_" << step << "process" << process << ".obj";

  std::ofstream file(name_file.str());

  file << " File Wavefront OBJ generated apMesh\n\n";

  const auto now = std::chrono::system_clock::now();
  const auto now_time_t = std::chrono::system_clock::to_time_t(now);
  struct tm now_tm;
  localtime_r(&now_time_t, &now_tm);
  file << "# File created: " << (now_tm.tm_year + 1900) << '-'
       << (now_tm.tm_mon + 1) << '-' << now_tm.tm_mday << '\n';

  std::uint64_t Nv = 0, Nt = 0;

  for (const auto& sub : mesh->GetSubMeshAdaptive()) {
    Nv += sub->GetNumberNos();
    Nt += sub->GetNumberElements();
  }

  file << "# of vertices\n" << Nv << "\n\n";

  for (const auto& sub : mesh->GetSubMeshAdaptive()) {
    for (const auto& n : sub->GetNos()) {
      file << "v " << n->GetX() << ' ' << n->GetY() << ' ' << n->GetZ() << '\n';
    }
  }

  file << "# of faces\n" << Nt << '\n';

  for (const auto& sub : mesh->GetSubMeshAdaptive()) {
    for (const auto& t : sub->GetElements()) {
      const auto* t_adaptive = dynamic_cast<const TriangleAdaptive*>(t.get());
      file << "f " << t_adaptive->GetNoh(1).GetId() << ' '
           << t_adaptive->GetNoh(2).GetId() << ' '
           << t_adaptive->GetNoh(3).GetId() << '\n';
    }
  }

  file.flush();
  file.close();

  return true;
}