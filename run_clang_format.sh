#!/bin/bash

THIS_PATH="$(realpath "$0")"
THIS_DIR="$(dirname "$THIS_PATH")"

# Adicionar os caminhos das pastas que deseja formatar aqui
PROJECT_DIRS=()
if [ -d "$THIS_DIR/ap_mesh_project/include" ]; then
  PROJECT_DIRS+=("$THIS_DIR/ap_mesh_project/include")
fi
if [ -d "$THIS_DIR/ap_mesh_project/src" ]; then
  PROJECT_DIRS+=("$THIS_DIR/ap_mesh_project/src")
fi

# Encontrar todos os arquivos nas pastas especificadas que terminam com .ino, .cpp, etc.
# e salvar a lista de arquivos em uma variável
FILE_LIST=""
for dir in "${PROJECT_DIRS[@]}"; do
  files=$(find "$dir" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.hh" -o -name "*.c" -o -name "*.cpp" -o -name "*.cc" \))
  FILE_LIST="$FILE_LIST\n$files"
done

# Exibir a lista de arquivos encontrados
echo -e "Files found to format = \n\"\"\"\n$FILE_LIST\n\"\"\""

# Formatar cada arquivo encontrado
clang-format --verbose -i --style=Google $FILE_LIST

