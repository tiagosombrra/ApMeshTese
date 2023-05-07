#!/bin/bash

THIS_PATH="$(realpath "$0")"
THIS_DIR="$(dirname "$THIS_PATH")"

# Find all files in THIS_DIR which end in .ino, .cpp, etc., as specified
# in the regular expression just below
FILE_LIST="$(find "$THIS_DIR" \( -path "*/libs/Eigen/*" -o -path "*/build/*" -o -path "*/build_make/*" -o -path "*/.codechecker/*" -o -path "*/input_meshes/*" -o -path "*/output_meshes/*" -o -path "*/qtcreator/*" -o -path "*/scripts/*" \) -prune -o \( -type f \( -name "*.cpp" -o -name "*.h" \) -a \( -path "*/include/*" -o -path "*/src/*" \) \) )"

echo -e "Files found to format = \n\"\"\"\n$FILE_LIST\n\"\"\""

# Format each file.
# - NB: do NOT put quotes around `$FILE_LIST` below or else the `clang-format` command will 
#   mistakenly see the entire blob of newline-separated file names as a SINGLE file name instead 
#   of as a new-line separated list of *many* file names!
clang-format --verbose -i --style=Google $FILE_LIST
