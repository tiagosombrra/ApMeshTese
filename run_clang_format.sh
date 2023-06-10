#!/bin/bash

THIS_PATH="$(realpath "$0")"
THIS_DIR="$(dirname "$THIS_PATH")"

# Find all files in THIS_DIR (excluding the 'libs', 'lib', 'build' and 'build_make' folders) which end in .ino, .cpp, etc., as specified
# in the regular expression just below
FILE_LIST="$(find "$THIS_DIR" -type d \( -name 'libs' -o -name 'lib' -o -name 'build' -o -name 'build_make' \) -prune -o -type f -regex '.*\.\(ino\|cpp\|c\|h\|hpp\|hh\)$')"

echo -e "Files found to format = \n\"\"\"\n$FILE_LIST\n\"\"\""

# Format each file.
# - NB: do NOT put quotes around `$FILE_LIST` below or else the `clang-format` command will 
#   mistakenly see the entire blob of newline-separated file names as a SINGLE file name instead 
#   of as a new-line separated list of *many* file names!
clang-format --verbose -i --style=Google $FILE_LIST
