//
//  main.cpp
//  genBezierPatches
//
//  Created by Tiago Guimarães Sombra on 12/11/15.
//  Copyright © 2015 TMeshSurf. All rights reserved.
//

#include <iostream>
#include "../header/basic/Definitions.h"
#include "../header/io/ReaderObjFiles.h"
#include "../header/generator/GenerateBezierPatch.h"
#include "../header/io/WritePTFile.h"

// argv[0] = "executavel: ./genPatches",
// argv[1] = "entrada",
// argv[2] = "saida",
int main(int argc, char* argv[]) {

std::string entrada;
std::string saida;
entrada = argv[1];
saida = argv[2];

    ReaderObjFiles reader;
#ifdef __APPLE__
    //VectorTuple48d vectorPoints = reader.fileReadingRIB("../../entrada/cone.rib");
  //  VectorTuple48d vectorPoints = reader.fileReadingOBJ("/Users/tiagosombra/Documents/teaspoon.obj");
   // VectorTuple48d vectorPoints = reader.readerObjOriginalTeapot("/Users/tiagosombra/Documents/teapot.obj");
    VectorTuple48d vectorPoints = reader.fileReadingOBJ_Surface_Order("/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/genBezierPatches/projeto/linux/entrada/monte_everest.obj");

#else
   // VectorTuple48d vectorPoints = reader.convertRIBintoOBJ("../../entrada/cone.rib");
  //  VectorTuple48d vectorPoints = reader.fileReadingOBJ("../../entrada/4_patches_subdivisao.obj");
    //VectorTuple48d vectorPoints = reader.fileReadingPatchesSiqueira("../../entrada/uteapot.siq");
    VectorTuple48d vectorPoints = reader.fileReadingOBJ_Surface_Order(entrada);
#endif //#ifdef __APPLE__

    GenerateBezierPatch gen;
    cout<<"INICIO >>> CALCULO"<<endl;
    vectorPoints = gen.calculateControlPoints(vectorPoints);
    cout<<"FIM >>> CALCULO"<<endl;

    cout<<"INICIO >>> REDUCEGEN"<<endl;
    vectorPoints = gen.reduceAllPatches(vectorPoints,2);
    cout<<"FIM >>> REDUCEGEN"<<endl;


    WritePTFile write;

#ifdef __APPLE__
    //write.writerPTFile("../../saida/cone.pt", vectorPoints);
    write.writeBPFile("/Users/tiagosombra/Dropbox/tiago/ufc/MestradoTiago/GitHub/TMeshSurf/genBezierPatches/projeto/linux/saida/monte_ruapehu.bp", reader.convertBP(vectorPoints));
    cout<<"SUCESSO >>> ARQUIVO SALVO."<<endl;
#else
    // genWriter.writerPatchBezier("/home/tiago/Ufc/GitHub/apMesh/auxiliares/modelagem_patches/exemplos/boneco.pt", genPatch.reduceAllPatches(genPatch.readerPatchesBezier(vectorPointsMesh), SizeSubDivPatches));
     //write.writerPTFile("../../saida/cone.pt", vectorPoints);
    write.writeBPFile(saida, reader.convertBP(vectorPoints));
    cout<<"SUCESSO >>> ARQUIVO SALVO."<<endl;
#endif //#ifdef __APPLE__

    return 0;
}
