BIN = bin
SRC = src
OBJ = obj
INCLUDE = include
COMPILER = -std=c++0x
OPTIMIZATION = -O2

CXX = g++

all: $(BIN)/tspbb $(BIN)/tspback $(BIN)/cenagala

Cenagala: $(BIN)/cenagala

TSP: $(BIN)/tspbb $(BIN)/tspback

$(BIN)/cenagala: $(OBJ)/cenagala.o
	@$(CXX) -o $(BIN)/cenagala $(OBJ)/cenagala.o $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando algoritmo Cena Gala..."

$(BIN)/tspbb: $(OBJ)/tspbb.o
	@$(CXX) -o $(BIN)/tspbb $(OBJ)/tspbb.o $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando algoritmo Branch and Bound..."
	@echo "Los casos de ejecución serán descomprimidos en la carpeta bin"
	@gunzip -k ./data/*.tsp.gz 
	@mv ./data/*.tsp ./bin/

$(BIN)/tspback: $(OBJ)/tspback.o
	@$(CXX) -o $(BIN)/tspback $(OBJ)/tspback.o $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando algoritmo Backtracking..."

$(OBJ)/tspbb.o: $(SRC)/tspbb.cpp
	@$(CXX) -c -o $(OBJ)/tspbb.o $(SRC)/tspbb.cpp $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando código objeto del algoritmo TSP B&B..."

$(OBJ)/tspback.o: $(SRC)/tspback.cpp
	@$(CXX) -c -o $(OBJ)/tspback.o $(SRC)/tspback.cpp $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando código objeto del algoritmo TSP Backtracking..."
	
$(OBJ)/cenagala.o : $(SRC)/cenagala.cpp
	@$(CXX) -c -o $(OBJ)/cenagala.o $(SRC)/cenagala.cpp $(COMPILER) $(OPTIMIZATION)
	@echo "Compilando código objeto de cena gala..."

clean:
	@echo "Eliminando ejecutables y ficheros objeto..."
	@rm -f $(OBJ)/* $(BIN)/*

mrproper:
	@echo "Eliminando casos de prueba..."
	@rm ./bin/*.tsp

