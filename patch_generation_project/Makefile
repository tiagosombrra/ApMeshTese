PROGRAMA = genPatch

FONTES = src/generator/GenerateBezierPatch.cpp \
src/io/ReaderObjFiles.cpp \
src/io/WritePTFile.cpp \
src/main.cpp

OBJETOS = $(FONTES:.cpp=.o)

CC            = gcc
CXX           = g++

INCPATH = -Iheaders/basic -Iheaders/generator -Iheaders/io

LIBPATH = -Lsrc/generator -Lsrc/io

CXXFLAGS_EXTRA = -O3 -W -Wall
CXXFLAGS      = -std=c++17 $(CXXFLAGS_EXTRA)

$(PROGRAMA): $(OBJETOS)
	@echo "gerando o executavel..."
	$(CXX) $(INCPATH) $(CXXFLAGS) $(LIBPATH) $(OBJETOS) -o $@

.cpp.o:
	@echo "gerando os .o..."
	$(CXX) $(INCPATH) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rfv *.o src/generator/*.o src/io/*.o $(PROGRAMA)

exec: 
	./$(PROGRAMA)

