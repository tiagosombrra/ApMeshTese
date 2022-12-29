/* Classe para leitura do arquivo com a descrição geométrica da superfície
 */
#ifndef Arquivo_h
#define Arquivo_h

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

#include "../data/model.h"

using namespace std;

class Arquivo {
  string nome;
  ifstream INPUT_MODEL;
  list<string> curvas;
  list<string> patches;

  void lerCurvas(const string leitura);   // lê todas as curvas do arquivo
  void lerPatches(const string leitura);  // lê todos os patches do arquivo
  void criarCurvasPara();                 // cria as curvas a partir da list
  void criarPatchesPara();                // cria os patches a partir da list
  char* convertaString(string fonte);

 public:
  string getNome();
  void lerArquivoPara(/*Modelo& modelo*/);
  Arquivo(const char* nome);
  ~Arquivo();
};
#endif
